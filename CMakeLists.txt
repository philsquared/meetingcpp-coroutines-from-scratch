cmake_minimum_required(VERSION 3.22)
project(NanoQuant)

set(CMAKE_CXX_STANDARD 20)

add_executable(NanoQuant main.cpp FObjects/FObject.h SimpleTypes/DayCounter.h SimpleTypes/Calendar.h SimpleTypes/Payoff.h FObjects/Curve.h SimpleTypes/Leg.h Enums/BusinessDayConvention.h Enums/Frequency.h SimpleTypes/Date.h FObjects/Bond.h FObjects/FixedRateBond.h FObjects/Forward.h FObjects/FixedRateBondForward.h Infrastructure/Serialisation.cpp Infrastructure/Serialisation.h Enums/Compounding.h FObjects/FittedBondDiscountCurve.h Builders/AllBuilders.h Builders/AllBuilders.cpp Infrastructure/Deserialiser.h FObjects/Swap.h FObjects/YearOnYearInflationSwap.h FObjects/Index.h FObjects/InflationIndex.h SimpleTypes/Region.h SimpleTypes/Period.h SimpleTypes/Currency.h SimpleTypes/Currencies.h FObjects/YoYInflationIndex.h FObjects/YoYInflationCurve.h FObjects/InflationCurve.h Infrastructure/Log.h Infrastructure/BuildTask.cpp Infrastructure/BuildTask.h Infrastructure/Dependencies.h Infrastructure/Dependency.h Infrastructure/Repo.cpp Infrastructure/Repo.h Infrastructure/TaskTracker.h Infrastructure/TaskTracker.cpp)
