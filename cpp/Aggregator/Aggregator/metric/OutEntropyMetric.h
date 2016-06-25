//
//  OutEntropyMetric.h
//  memNet
//
//  Created by Christian Persson on 12/01/15.
//  Copyright (c) 2015 Christian Persson. All rights reserved.
//

#ifndef __memNet__OutEntropyMetric__
#define __memNet__OutEntropyMetric__

#include "Metric.h"

class OutEntropyMetric : public Metric {
public:
    double metric(Aggregate &a) {
        double t = a.H_out * a.p_out;
        return t > 0.0 ? t : 0.0;
    }

    double metric_increase(Aggregate &a, Aggregate &b) {
        double d = entropyMap2(a.outWeigths, b.outWeigths) * (a.p_out + b.p_out) - metric(a) - metric(b);
        return d > 0.0 ? d : 0.0;
    }
};

#endif /* defined(__memNet__OutEntropyMetric__) */
