cmake_minimum_required(VERSION 3.22)
project(NanoQuant)

set(CMAKE_CXX_STANDARD 20)

add_executable(NanoQuant main.cpp FObjects/FObject.h SimpleTypes/DayCounter.h SimpleTypes/Calendar.h SimpleTypes/Payoff.h FObjects/Curve.h SimpleTypes/Leg.h Enums/BusinessDayConvention.h Enums/Frequency.h SimpleTypes/Date.h FObjects/Bond.h FObjects/FixedRateBond.h FObjects/Forward.h FObjects/FixedRateBondForward.h Infrastructure/Repo.h Infrastructure/Repo.cpp Infrastructure/Serialisation.cpp Infrastructure/Serialisation.h Enums/Compounding.h FObjects/FittedBondDiscountCurve.h)
