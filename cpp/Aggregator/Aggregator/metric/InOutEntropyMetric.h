//
//  InOutEntropyMetric.h
//  memNet
//
//  Created by Christian Persson on 12/01/15.
//  Copyright (c) 2015 Christian Persson. All rights reserved.
//

#ifndef __memNet__InOutEntropyMetric__
#define __memNet__InOutEntropyMetric__

#include "Metric.h"

class InOutEntropyMetric : public Metric {
public:
    double metric(Aggregate &a) {
        return createEntropyData_local(a.p_in + a.p_out, a.H_in + a.H_out).H;
    }

    double metric_increase(Aggregate &a, Aggregate &b) {
        double ap = a.p_in + a.p_out;
        double bp = b.p_in + b.p_out;
        return entropyMap22(a.outWeigths, b.outWeigths, a.inWeights, b.inWeights) * (ap + bp) - metric(a) * ap - metric(b) * bp;
    }
};


#endif /* defined(__memNet__InOutEntropyMetric__) */
