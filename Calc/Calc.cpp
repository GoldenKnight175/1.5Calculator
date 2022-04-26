#include "Calc.h"

#include <string>




namespace WxCalc
{
    double Calculator::performOperation(double const left,double const right,int const op)
    {
        double ans;
        switch (op) {
        case Add: {
            ans = left+right;
            break;
        }
        case Sub: {
            ans = left-right;
            break;
        }
        case Mult: {
            ans = left*right;
            break;
        }
        case Div: {
            if (right != 0) {
                ans = left/right;
            }
            else {
                throw std::runtime_error("Cannot divide by zero.");
            }
            break;
        }
        default: {
            throw std::runtime_error("Unknown OP: " + std::to_string(op));
        }
        }
        return ans;
    }
}