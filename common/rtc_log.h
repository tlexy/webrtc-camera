#include <iostream>

#define RTC_LOG(level) std::cout
#define RTC_DLOG(level) std::cout
#define RTC_DCHECK(ptr) /*if(ptr == nullptr) std::cout << "ptr is nullptr" << std::endl;*/
#define RTC_DCHECK_RUN_ON(ptr)
#define RTC_DCHECK_NOTREACHED(ptr)
#define RTC_DCHECK_EQ(a, b)