#pragma once

#include "FrameLab/MichelsonMorleyTypes.h"

namespace framelab {

class IMichelsonMorleyModel {
public:
    virtual ~IMichelsonMorleyModel() = default;

    [[nodiscard]] virtual MichelsonMorleyResult simulate(
        const MichelsonMorleyParameters& parameters) const = 0;
};

} // namespace framelab
