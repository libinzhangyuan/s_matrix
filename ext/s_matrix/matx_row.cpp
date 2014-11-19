#include "matx_row.h"
#include "stdio.h"

MatxRow MatxRow::null_row = MatxRow();
std::string MatxRow::null_string(")(=+matx_null#!^&");
static std::string empty_string;

MatxRow::MatxRow(void)
{
    printf("---MatxRow new\n");
}

MatxRow::MatxRow(const MatxRow& r)
{
    *this = r;
}

MatxRow& MatxRow::operator=(const MatxRow& r)
{
    if (&r == this)
       return *this;
    set_values(r.get_values());
    return *this;
}

MatxRow::MatxRow(const std::vector<std::string>& values)
{
    set_values(values);
}

MatxRow::~MatxRow()
{
    reset_container();
}

void MatxRow::reset_container(void)
{
    for (size_t i = 0; i < m_values.size(); ++i)
    {
        std::string* pStr = m_values[i];
        if (pStr != &null_string && pStr != &empty_string)
            delete pStr;
    }
    m_values.clear();
}

void MatxRow::set_values(const std::vector<std::string>& values)
{
    reset_container();
    for (size_t i = 0; i < values.size(); ++i)
    {
        const std::string& value = values[i];
        if (value.size() == 0)
            m_values.push_back(&empty_string);
        else if (value == MatxRow::null_string)
            m_values.push_back(&null_string);
        else
        {
            std::string* pValue = new std::string(value);
            m_values.push_back(pValue);
        }
    }
}

std::vector<std::string> MatxRow::get_values(void) const
{
    std::vector<std::string> values;
    for (size_t i = 0; i < m_values.size(); ++i)
        values.push_back(*m_values[i]);
    return values;
}

const std::string& MatxRow::operator[](size_t index) const
{
    if (index >= m_values.size())
        return null_string;
    return *m_values[index];
}
