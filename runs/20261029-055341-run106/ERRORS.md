# Sixfaced CI - FAILED run #106

- run URL : https://github.com/NothingNormal/Kernel/actions/runs/40825865779
- ref     : refs/heads/arena/0fc62de9-kernel
- commit  : fb361c0e9b775160bcba71918471cba8e3231440
- date    : 2026-10-29 05:53:41 UTC
- clang   : Ubuntu clang version 21.1.8 (6ubuntu1)

## Extracted errors (last 300 matches)
```
2:/tmp/qcom-charge-test-dm6ln_58/qcom_charge_types.h:78:9: error: unknown type name ‘spinlock_t’; did you mean ‘clock_t’?
```

## Tail of build log (last 250 lines)
```
In file included from /home/runner/work/Kernel/Kernel/tools/testing/selftests/power_supply/qcom_charge_mock.c:39:
/tmp/qcom-charge-test-dm6ln_58/qcom_charge_types.h:78:9: error: unknown type name ‘spinlock_t’; did you mean ‘clock_t’?
   78 |         spinlock_t              status_change_lock;
      |         ^~~~~~~~~~
      |         clock_t
Traceback (most recent call last):
  File "/home/runner/work/Kernel/Kernel/tools/testing/selftests/power_supply/qcom_charge_host_test.py", line 136, in <module>
    main()
    ~~~~^^
  File "/home/runner/work/Kernel/Kernel/tools/testing/selftests/power_supply/qcom_charge_host_test.py", line 128, in main
    subprocess.run(cc + flags + ["-I", str(temp), str(MOCK), "-o", str(exe)],
    ~~~~~~~~~~~~~~^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                   check=True)
                   ^^^^^^^^^^^
  File "/usr/lib/python3.14/subprocess.py", line 578, in run
    raise CalledProcessError(retcode, process.args,
                             output=stdout, stderr=stderr)
subprocess.CalledProcessError: Command '['cc', '-std=gnu11', '-O1', '-g', '-fno-omit-frame-pointer', '-fsanitize=address,undefined', '-fno-sanitize-recover=all', '-I', '/tmp/qcom-charge-test-dm6ln_58', '/home/runner/work/Kernel/Kernel/tools/testing/selftests/power_supply/qcom_charge_mock.c', '-o', '/tmp/qcom-charge-test-dm6ln_58/qcom_charge_mock']' returned non-zero exit status 1.
```
