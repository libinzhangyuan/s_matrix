#ifndef _GMATX_H_
#define _GMATX_H_

class GMatx
{
public:
    GMatx(void);

    int get_i(void) {return m_i;};
    void set_i(const int i) {m_i = i;};

private:
    int m_i;
};



#endif
