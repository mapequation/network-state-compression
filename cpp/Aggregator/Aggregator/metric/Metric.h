//
//  Metric.h
//  memNet
//
//  Created by Christian Persson on 12/01/15.
//  Copyright (c) 2015 Christian Persson. All rights reserved.
//

#ifndef __memNet__Metric__
#define __memNet__Metric__

#include <stdio.h>
#include "../Aggregate.h"

class Metric {
public:
    virtual double metric(Aggregate &a) // could be pure virtual, if you like
    {
        std::cout << "SuperClass::foo()" << std::endl;
        return 0;
    }

    virtual double metric_increase(Aggregate &a, Aggregate &b) {
        std::cout << "SuperClass::foo()" << std::endl;
        return 0;
    }

};

#endif /* defined(__memNet__Metric__) */
