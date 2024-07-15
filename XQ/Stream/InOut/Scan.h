/**
  ******************************************************************************
  * @file           : Scan.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/6/3
  ******************************************************************************
  */

#ifndef MYCONTAINER_SCAN_H
#define MYCONTAINER_SCAN_H

#include <tuple>

#include "../../Tools/Expected.h"

namespace XQ::Stream {
    enum class ScanError {

    };

    namespace InOut {
        template<typename ...Args>
        class ScanResult {
        public:
            auto values() const {
                return _val;
            }

        private:
            std::tuple<Args...> _val;
        };

        template<typename ...Args, typename Range>
        XQ::Tools::Expected<ScanResult<Args...>, ScanError> scan(Range &&range) {
            //
        }
    }
}

#endif //MYCONTAINER_SCAN_H
