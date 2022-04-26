#pragma once

#ifndef WXCALC_SRC_CALC_HPP
#define WXCALC_SRC_CALC_HPP
#include <stdexcept>
#include <cstdint>
namespace WxCalc
{
    class Calculator
    {
    public:
        enum operation_type {
            Add,
            Sub,
            Mult,
            Div
        };
        static double performOperation(double left,double right,int op);
    };
}
#endif