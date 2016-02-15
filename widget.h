#ifndef WIDGET_H
#define WIDGET_H

class Widget
{
public:
    virtual void retranslateStrings() = 0;
    virtual void setupScheme() = 0;
protected:
    virtual void createActions() = 0;
};

#endif // WIDGET_H
