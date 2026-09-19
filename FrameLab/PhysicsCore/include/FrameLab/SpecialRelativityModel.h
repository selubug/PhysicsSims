#pragma once

#include "FrameLab/IMichelsonMorleyModel.h"

namespace framelab {

// Special relativity evaluated in the interferometer's rest frame.
// Both equal arms have round-trip light time 2L/c for every orientation.
class SpecialRelativityModel final : public IMichelsonMorleyModel {
public:
    [[nodiscard]] MichelsonMorleyResult simulate(
        const MichelsonMorleyParameters& parameters) const override;
};

} // namespace framelab
