#ifndef _MATX_TITLE_H_
#define _MATX_TITLE_H_

#include <string>
#include <vector>

class MatxTitle
{
public:
    MatxTitle(void);

    void add_title(const std::string& title);
    bool is_title_exist(const std::string& title) const;
    const std::vector<std::string>& get_titles(void) const;
    const std::string& operator[](size_t index) const;

private:
    int m_i;
    std::vector<std::string> m_titles;
};



#endif
