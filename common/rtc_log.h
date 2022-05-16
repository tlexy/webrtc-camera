#include <iostream>
#include <assert.h>

#define RTC_LOG(level) std::cout
#define RTC_DLOG(level) std::cout
#define RTC_CHECK(ptr)
#define RTC_DCHECK(ptr) /*if(ptr == nullptr) std::cout << "ptr is nullptr" << std::endl;*/
#define RTC_DCHECK_RUN_ON(ptr)
#define RTC_DCHECK_NOTREACHED(ptr)
#define RTC_DCHECK_EQ(a, b) assert((a) == (b))
#define RTC_DCHECK_GT(a, b) assert((a) > (b))
#define RTC_DCHECK_GE(a, b) assert((a) >= (b))
#define RTC_DCHECK_LE(a, b) assert((a) <= (b))
#define RTC_CHECK_EQ(a, b)
#define RTC_CHECK_LE(a, b)
#define RTC_CHECK_GE(a, b)