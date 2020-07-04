def FeatureFloat : SubtargetFeature<"float", "SupportFloat", "true", "Enable floating point operations">;

def CPU0P1 : Processor<"CPU0", CPU0GenericItineraries, [ FeatureFloat ]>;
