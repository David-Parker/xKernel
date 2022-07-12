#include <time.h>
#include <timer.h>
#include <tsc.h>

static _u64 last_timestamp_ns = 0;

_u64 ktime_get_timer_ns()
{
    return (_u64)(((double)timer_ticks/TIMER_HZ)*NANOS_PER_SEC);
}

_u64 ktime_get_ns()
{
    _u64 tsc = read_tsc();
    _u64 delta = tsc - tsc_last;
    _u64 nanos_tsc_delta = (delta * NANOS_PER_SEC) / get_tsc_freq();

    _u64 timestamp = ktime_get_timer_ns() + nanos_tsc_delta;

    // Clock jumped "backwards". This can happen because the TSC and system clock
    // run at different frequencies, since we use the TSC (higher resoltion)
    // to interpolate between timer ticks (which are 50Hz), there may be cases
    // where time seems to jump backwards a few ns when we reset the TSC delta
    // and update to the latest system clock tick. This should be rare, and since
    // we need to guaruntee monotonicity, we return the previous timestamp value.
    // We cannot use just the TSC for wall clock time because it drifts faster than
    // the system clock. However, the system clock does drift and will get out of
    // sync with real wall clock time. This is the reason real kernels use NTP to
    // sync the clock periodically.
    if (timestamp < last_timestamp_ns)
    {
        return last_timestamp_ns;
    }
    else
    {
        last_timestamp_ns = timestamp;
        return timestamp;
    }
}

_u64 ktime_get_us()
{
    return ktime_get_ns() / 1000; 
}

_u64 ktime_get_ms()
{
    return ktime_get_us() / 1000;
}

_u64 ktime_get_s()
{
    return ktime_get_ms() / 1000;
}