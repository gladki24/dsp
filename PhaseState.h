//
// Created by Seweryn on 29.11.2020.
//

#ifndef DSP_PHASESTATE_H
#include "Consts.h"
#include <cmath>
#define DSP_PHASESTATE_H

// Declaration
class PhaseState {
public:
    virtual ~PhaseState() = default;

    virtual PhaseState* nextPi(PhaseState* prevState) {
        delete prevState;
        return nullptr;
    }

    virtual PhaseState* nextHalfPi(PhaseState* prevState) {
        delete prevState;
        return nullptr;
    }

    virtual void calc(double& phase, short factor) = 0;
};

class APhaseState : public PhaseState {
public:
    ~APhaseState() override = default;
    PhaseState *nextPi(PhaseState* prevState) override;
    PhaseState *nextHalfPi(PhaseState* prevState) override;
    void calc(double &phase, short factor) override;
};

class BPhaseState : public PhaseState {
public:
    ~BPhaseState() override = default;
    PhaseState *nextHalfPi(PhaseState* prevState) override;
    void calc(double &phase, short factor) override;
};

class CPhaseState : public PhaseState {
public:
    ~CPhaseState() override = default;
    PhaseState *nextPi(PhaseState* prevState) override;
    PhaseState *nextHalfPi(PhaseState* prevState) override;
    void calc(double &phase, short factor) override;
};

class DPhaseState : public PhaseState {
public:
    ~DPhaseState() override = default;
    PhaseState *nextHalfPi(PhaseState* prevState) override;
    void calc(double &phase, short factor) override;
};

// implementation A

PhaseState * APhaseState::nextPi(PhaseState *prevState) {
    delete prevState;
    return new BPhaseState();
}

PhaseState * APhaseState::nextHalfPi(PhaseState *prevState) {
    delete prevState;
    return new APhaseState();
}

void APhaseState::calc(double &phase, short factor) {
    phase += M_PI * OUTPUT_FREQUENCY / FREQUENCY_RANGE * factor;
}

// implementation B

PhaseState * BPhaseState::nextHalfPi(PhaseState *prevState) {
    delete prevState;
    return new CPhaseState();
}

void BPhaseState::calc(double &phase, short factor) {
    phase -= M_PI * OUTPUT_FREQUENCY / FREQUENCY_RANGE * factor;
}

// implementation C

PhaseState * CPhaseState::nextPi(PhaseState *prevState) {
    delete prevState;
    return new DPhaseState();
}

PhaseState * CPhaseState::nextHalfPi(PhaseState *prevState) {
    delete prevState;
    return new CPhaseState();
}

void CPhaseState::calc(double &phase, short factor) {
    phase += M_PI * OUTPUT_FREQUENCY / FREQUENCY_RANGE * factor;
}

// implementation D

PhaseState * DPhaseState::nextHalfPi(PhaseState *prevState) {
    delete prevState;
    return new DPhaseState();
}

void DPhaseState::calc(double &phase, short factor) {
    phase -= M_PI * OUTPUT_FREQUENCY / FREQUENCY_RANGE * factor;
}
#endif //DSP_PHASESTATE_H
