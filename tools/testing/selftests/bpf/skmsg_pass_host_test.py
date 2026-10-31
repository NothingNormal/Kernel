#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-only
"""Compile the real sk_psock_verdict_apply() with host-side ownership mocks."""

from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
SOURCE = ROOT / "net/core/skmsg.c"


PREAMBLE = r"""
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define __SK_PASS 0
#define __SK_DROP 1
#define __SK_REDIRECT 2
#define SOCK_DEAD 1
#define SK_PSOCK_TX_ENABLED 1
#define BPF_F_INGRESS 1
#define unlikely(x) (x)
#define TCP_SKB_CB(skb) (&(skb)->cb)

struct sock {
	int dead;
	int sk_rmem_alloc;
	int sk_rcvbuf;
};
struct tcp_skb_cb { struct { unsigned int flags; } bpf; };
struct sk_buff {
	struct tcp_skb_cb cb;
	int freed;
	int queued;
};
struct sk_psock {
	struct sock *sk;
	int tx_enabled;
	int ingress_skb;
	int work;
};

static int redirect_calls;
static int sock_flag(const struct sock *sk, int flag)
{
	(void)flag;
	return sk->dead;
}
static int sk_psock_test_state(const struct sk_psock *psock, int state)
{
	(void)state;
	return psock->tx_enabled;
}
static int atomic_read(const int *value) { return *value; }
static void skb_queue_tail(int *queue, struct sk_buff *skb)
{
	++*queue;
	skb->queued = 1;
}
static void schedule_work(int *work) { ++*work; }
static void kfree_skb(struct sk_buff *skb) { skb->freed = 1; }
static void sk_psock_skb_redirect(struct sk_psock *psock, struct sk_buff *skb)
{
	(void)psock;
	(void)skb;
	++redirect_calls;
}
"""


HARNESS = r"""
static struct sk_psock make_psock(struct sock *sk)
{
	struct sk_psock psock = { .sk = sk, .tx_enabled = 1 };
	return psock;
}

int main(void)
{
	struct sock sk = { .sk_rmem_alloc = 50, .sk_rcvbuf = 100 };
	struct sk_psock psock = make_psock(&sk);
	struct sk_buff skb = { 0 };

	sk_psock_verdict_apply(&psock, &skb, __SK_PASS);
	assert(skb.queued && !skb.freed);
	assert(psock.ingress_skb == 1 && psock.work == 1);
	assert(skb.cb.bpf.flags & BPF_F_INGRESS);

	/* Dead sockets, disabled psock TX, and receive-buffer overflow drop. */
	sk.dead = 1;
	skb = (struct sk_buff){ 0 };
	sk_psock_verdict_apply(&psock, &skb, __SK_PASS);
	assert(skb.freed && !skb.queued);
	sk.dead = 0;
	psock.tx_enabled = 0;
	skb = (struct sk_buff){ 0 };
	sk_psock_verdict_apply(&psock, &skb, __SK_PASS);
	assert(skb.freed && !skb.queued);
	psock.tx_enabled = 1;
	sk.sk_rmem_alloc = 101;
	skb = (struct sk_buff){ 0 };
	sk_psock_verdict_apply(&psock, &skb, __SK_PASS);
	assert(skb.freed && !skb.queued);

	/* Drop and unknown verdicts free; redirect delegates ownership. */
	sk.sk_rmem_alloc = 50;
	skb = (struct sk_buff){ 0 };
	sk_psock_verdict_apply(&psock, &skb, __SK_DROP);
	assert(skb.freed);
	skb = (struct sk_buff){ 0 };
	sk_psock_verdict_apply(&psock, &skb, 99);
	assert(skb.freed);
	skb = (struct sk_buff){ 0 };
	sk_psock_verdict_apply(&psock, &skb, __SK_REDIRECT);
	assert(redirect_calls == 1 && !skb.freed && !skb.queued);

	puts("skmsg verdict ownership cases: PASS");
	return 0;
}
"""


def function_body(source: str) -> str:
    marker = "static void sk_psock_verdict_apply("
    start = source.index(marker)
    opening = source.index("{", start)
    depth = 0
    for pos in range(opening, len(source)):
        if source[pos] == "{":
            depth += 1
        elif source[pos] == "}":
            depth -= 1
            if depth == 0:
                return source[start:pos + 1]
    raise ValueError("unterminated sk_psock_verdict_apply()")


def main() -> None:
    compiler = shutil.which("cc") or shutil.which("gcc") or shutil.which("clang")
    if not compiler:
        raise SystemExit("no host C compiler found")
    body = function_body(SOURCE.read_text())
    with tempfile.TemporaryDirectory(prefix="skmsg-pass-test-") as tmp:
        c_file = Path(tmp) / "test.c"
        binary = Path(tmp) / "test"
        c_file.write_text(PREAMBLE + "\n" + body + "\n" + HARNESS)
        subprocess.run([compiler, "-std=gnu11", "-Wall", "-Wextra", "-Werror",
                        "-fsanitize=address,undefined", str(c_file), "-o", str(binary)],
                       check=True)
        subprocess.run([str(binary)], check=True)


if __name__ == "__main__":
    main()
