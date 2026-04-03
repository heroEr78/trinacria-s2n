#pragma once

#ifdef TRINACRIA_DEBUG
#define TRCN_LOG(m) std::cout << (m)
#define TRCN_ERROR(m) std::cerr << (m)
#else 
#define TRCN_LOG(m)
#define TRCN_ERROR(m)
#endif
