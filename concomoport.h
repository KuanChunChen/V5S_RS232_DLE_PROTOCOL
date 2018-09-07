ifndef CONCOMOPORT_H
#define CONCOMOPORT_H


class concomport{


public:
//    connectPort(int);
    Ball();
    Ball(double, const char*);
    Ball(double, string&);

    double radius();
    string& name();

    void radius(double);
    void name(const char*);
    void name(string&);

    double volumn();

private:
    double _radius; // 半徑
    string _name; // 名稱
};




#endif // CONCOMOPORT_H
