//
// Created by 裴沛东 on 2021/11/30.
//

#ifndef LOG_LOGBUFFER_H
#define LOG_LOGBUFFER_H

#include <cstddef>
#include <cstring>
#include <string>

namespace Log {
    namespace buffer {
        const int SmallBuffer = 4000;
        const int LargeBuffer = 2000;
        template<int SIZE>
        class buffer {
        public:
            buffer() {
                m_cur = m_data;

            }
            void append(const char* buf,size_t len) {
                if(avail() > len) {
                    memcpy(m_cur,buf,len);
                    m_cur += len;
                }

            }
            const char* data() const {return m_data;}
            int length() const {return static_cast<int>(m_cur - m_data);}
            char* current() const {return m_cur;}
            void add(size_t len) { m_cur += len;}
            void reset() {m_cur = m_data;}
            void bzero() { memset(m_data,0,sizeof(m_data));}
            size_t avail() const {return end()-m_cur;}
            std::string toString() {return std::string(m_data,length());}
        private:
            const char* end() const {return m_data+sizeof(m_data);}
            char m_data[SIZE];
            char* m_cur;

        };

    }
}
#endif //LOG_LOGBUFFER_H
