#ifndef _MATX_ROW_H_
#define _MATX_ROW_H_

#include <string>
#include <vector>
#include "matx_string.h"
#include "matx_type_def.h"

class MatxRow
{
public:
    static MatxRow null_row;
    static std::string null_string;

    MatxRow(void);
    MatxRow(const MatxRow& r);
    MatxRow(const std::vector<std::string>& values);
    ~MatxRow();

    MatxRow& operator=(const MatxRow& r);

    void set_values(const std::vector<std::string>& values);
    std::vector<std::string> get_values(void) const;
    const std::string& operator[](size_t index) const;
    t_key_value_hash to_key_value_hash(const std::vector<std::string>& titles) const;

private:
    void reset_container(void);

private:
    std::vector<matx_string*> m_values; // values in one row
};



#endif  // _MATX_ROW_H_
