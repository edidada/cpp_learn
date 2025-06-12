#ifndef AA_H
#define AA_H

//extern void  Disp2();
class AA
{
public:
    friend void  Disp(AA & a);
//    friend void  Disp2();
    AA(int t);
    virtual ~AA();
protected:
private:
    int data;
    static int data2;
};

#endif // AA_H
