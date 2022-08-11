#ifndef NANDFLASH_SIMULATION_DATA_GENERATOR
#define NANDFLASH_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class NANDFlashAnalyzerSettings;

class NANDFlashSimulationDataGenerator
{
public:
	NANDFlashSimulationDataGenerator();
	~NANDFlashSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, NANDFlashAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	NANDFlashAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //NANDFLASH_SIMULATION_DATA_GENERATOR