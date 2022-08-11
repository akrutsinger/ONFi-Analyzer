#include "NANDFlashAnalyzer.h"
#include "NANDFlashAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

NANDFlashAnalyzer::NANDFlashAnalyzer()
	: Analyzer2(),
	mLastCommand(0x00),
	mDataIsOutput(false),
	mSettings(new NANDFlashAnalyzerSettings()),
	mSimulationInitilized(false),
	mReadEnable(NULL),
	mWriteEnable(NULL),
	mIO0(NULL),
	mIO1(NULL),
	mIO2(NULL),
	mIO3(NULL),
	mIO4(NULL),
	mIO5(NULL),
	mIO6(NULL),
	mIO7(NULL)
{
	SetAnalyzerSettings(mSettings.get());
}

NANDFlashAnalyzer::~NANDFlashAnalyzer()
{
	KillThread();
}

void NANDFlashAnalyzer::SetupResults()
{
	mResults.reset(new NANDFlashAnalyzerResults(this, mSettings.get()));
	SetAnalyzerResults(mResults.get());

	if (mSettings->mReadEnableChannel != UNDEFINED_CHANNEL)
	{
		mResults->AddChannelBubblesWillAppearOn(mSettings->mReadEnableChannel);
	}
	if (mSettings->mWriteEnableChannel != UNDEFINED_CHANNEL)
	{
		mResults->AddChannelBubblesWillAppearOn(mSettings->mWriteEnableChannel);
	}
	if (mSettings->mIO0Channel != UNDEFINED_CHANNEL)
	{
		mResults->AddChannelBubblesWillAppearOn(mSettings->mIO0Channel);
	}
	if (mSettings->mIO1Channel != UNDEFINED_CHANNEL)
	{
		mResults->AddChannelBubblesWillAppearOn(mSettings->mIO1Channel);
	}
	if (mSettings->mIO2Channel != UNDEFINED_CHANNEL)
	{
		mResults->AddChannelBubblesWillAppearOn(mSettings->mIO2Channel);
	}
	if (mSettings->mIO3Channel != UNDEFINED_CHANNEL)
	{
		mResults->AddChannelBubblesWillAppearOn(mSettings->mIO3Channel);
	}
	if (mSettings->mIO4Channel != UNDEFINED_CHANNEL)
	{
		mResults->AddChannelBubblesWillAppearOn(mSettings->mIO4Channel);
	}
	if (mSettings->mIO5Channel != UNDEFINED_CHANNEL)
	{
		mResults->AddChannelBubblesWillAppearOn(mSettings->mIO5Channel);
	}
	if (mSettings->mIO6Channel != UNDEFINED_CHANNEL)
	{
		mResults->AddChannelBubblesWillAppearOn(mSettings->mIO6Channel);
	}
	if (mSettings->mIO7Channel != UNDEFINED_CHANNEL)
	{
		mResults->AddChannelBubblesWillAppearOn(mSettings->mIO7Channel);
	}
}

void NANDFlashAnalyzer::WorkerThread()
{
	Setup();

	AdvanceToReadOrWriteEnableEdge();

	for( ; ; )
	{
		/* 1. Check state: Reading, Writing, or UNKNOWN */
		
		/* 2. Get the current data (byte) */
		GetByte();
	}
}

void NANDFlashAnalyzer::Setup(void)
{
	mReadEnable = GetAnalyzerChannelData(mSettings->mReadEnableChannel);
	mWriteEnable = GetAnalyzerChannelData(mSettings->mWriteEnableChannel);
	mIO0 = GetAnalyzerChannelData(mSettings->mIO0Channel);
	mIO1 = GetAnalyzerChannelData(mSettings->mIO1Channel);
	mIO2 = GetAnalyzerChannelData(mSettings->mIO2Channel);
	mIO3 = GetAnalyzerChannelData(mSettings->mIO3Channel);
	mIO4 = GetAnalyzerChannelData(mSettings->mIO4Channel);
	mIO5 = GetAnalyzerChannelData(mSettings->mIO5Channel);
	mIO6 = GetAnalyzerChannelData(mSettings->mIO6Channel);
	mIO7 = GetAnalyzerChannelData(mSettings->mIO7Channel);
}

void NANDFlashAnalyzer::AdvanceToReadOrWriteEnableEdge(void)
{
	U64 current_sample = 0;
	U64 next_read_enable_sample = 0;
	U64 next_write_enable_sample = 0;
	U64 next_sample = 0;

	if (mReadEnable != NULL)
	{
		current_sample = mReadEnable->GetSampleNumber();
		next_read_enable_sample = mReadEnable->GetSampleOfNextEdge();
		/* Only 1ns between reads */
		if (1 == (next_read_enable_sample - current_sample))
		{
			AdvanceToReadEnableHighEdge();	// Skip past the nano-second glitch. NOTE: We can't use Logic's glitch filter because some of these glitches are back-to-back and the filter would get rid of the entire read
			next_read_enable_sample = mReadEnable->GetSampleOfNextEdge();
		}
	}

	if (mWriteEnable != NULL)
	{
		next_write_enable_sample = mWriteEnable->GetSampleOfNextEdge();
	}

	/* Samples are time based so lowest value is next sample */
	if (next_read_enable_sample <= next_write_enable_sample)
	{
		AdvanceToReadEnableHighEdge();	// Reads happen on the falling edge of the ReadEnable line

		/* NOTE: According to the datasheet, there is a read access time, tREA, ranging
		 * between 0 - 16ns. We'll try reading on the rising edge of the signal, which is usually
		 * between 20-30ns in order to ensure we make it past the read access time.
		 */
		next_sample = mReadEnable->GetSampleNumber();

		mDataIsOutput = true;
	}
	else
	{
		AdvanceToWriteEnableHighEdge();	// Writes happen on the rising edge of the WriteEnable line
		next_sample = mWriteEnable->GetSampleNumber();
		mDataIsOutput = false;
	}

	SynchronizeAllChannels(next_sample);
}

void NANDFlashAnalyzer::AdvanceToReadEnableHighEdge(void)
{
	if (mReadEnable != NULL)
	{
		mReadEnable->AdvanceToNextEdge();
		
		/* If we advance to a high edge, advance again to the next edge, which will be low */
		if (mReadEnable->GetBitState() != BIT_HIGH)
		{
			mReadEnable->AdvanceToNextEdge();
		}

		mResults->AddMarker(mReadEnable->GetSampleNumber(), AnalyzerResults::UpArrow, mSettings->mReadEnableChannel);
		mResults->CommitResults();
	}
}

void NANDFlashAnalyzer::AdvanceToWriteEnableHighEdge(void)
{
	if (mWriteEnable != NULL)
	{
		mWriteEnable->AdvanceToNextEdge();

		/* If we advance to a high edge, advance again to the next edge, which will be low */
		if (mWriteEnable->GetBitState() != BIT_HIGH)
		{
			mWriteEnable->AdvanceToNextEdge();
		}

		mResults->AddMarker(mWriteEnable->GetSampleNumber(), AnalyzerResults::UpArrow, mSettings->mWriteEnableChannel);
		mResults->CommitResults();
	}
}

void NANDFlashAnalyzer::SynchronizeAllChannels(U64 sample_number)
{
	mIO0->AdvanceToAbsPosition(sample_number);
	mIO1->AdvanceToAbsPosition(sample_number);
	mIO2->AdvanceToAbsPosition(sample_number);
	mIO3->AdvanceToAbsPosition(sample_number);
	mIO4->AdvanceToAbsPosition(sample_number);
	mIO5->AdvanceToAbsPosition(sample_number);
	mIO6->AdvanceToAbsPosition(sample_number);
	mIO7->AdvanceToAbsPosition(sample_number);
	mReadEnable->AdvanceToAbsPosition(sample_number);
	mWriteEnable->AdvanceToAbsPosition(sample_number);
}

void NANDFlashAnalyzer::GetByte(void)
{
	U8 data = 0;
	bool save_data = false;
	U64 starting_sample = mIO0->GetSampleNumber();	// NOTE: It doesn't matter which channel sample we get here; all channels are sync'd in "AdvanceToReadOrWriteEnableEdge"

	/* Get the data */
	data = mIO0->GetBitState();
	data |= mIO1->GetBitState() << 1;
	data |= mIO2->GetBitState() << 2;
	data |= mIO3->GetBitState() << 3;
	data |= mIO4->GetBitState() << 4;
	data |= mIO5->GetBitState() << 5;
	data |= mIO6->GetBitState() << 6;
	data |= mIO7->GetBitState() << 7;

	/*TODO: Implement logic to determine if the next sample we want is for reading data or writing a command/address */

	/* If ReadEnable line is low we're writing data */
	if (mDataIsOutput == true
		&& mWriteEnable->GetBitState() == BIT_HIGH
		&& (mLastCommand == 0x30		// READ PAGE
			|| mLastCommand == 0x31		// READ PAGE CACHE RANDOM
			|| mLastCommand == 0x3F		// READ PAGE CACHE LAST
			|| mLastCommand == 0xE0))	// RANDOM DATA READ
	{
		save_data = true;
	}
	else if (mDataIsOutput == false)	// Write command
	{
		mLastCommand = data;
	}


	AdvanceToReadOrWriteEnableEdge();


	if (save_data == true)
	{
		/* Draw a dot on each IO line where the sample was taken */
		mResults->AddMarker(starting_sample, AnalyzerResults::Dot, mSettings->mIO0Channel);
		mResults->AddMarker(starting_sample, AnalyzerResults::Dot, mSettings->mIO1Channel);
		mResults->AddMarker(starting_sample, AnalyzerResults::Dot, mSettings->mIO2Channel);
		mResults->AddMarker(starting_sample, AnalyzerResults::Dot, mSettings->mIO3Channel);
		mResults->AddMarker(starting_sample, AnalyzerResults::Dot, mSettings->mIO4Channel);
		mResults->AddMarker(starting_sample, AnalyzerResults::Dot, mSettings->mIO5Channel);
		mResults->AddMarker(starting_sample, AnalyzerResults::Dot, mSettings->mIO6Channel);
		mResults->AddMarker(starting_sample, AnalyzerResults::Dot, mSettings->mIO7Channel);
		
		/* Save the data */
		Frame frame;
		frame.mData1 = data;
		frame.mFlags = 0;
		frame.mStartingSampleInclusive = starting_sample;
		frame.mEndingSampleInclusive = mIO0->GetSampleNumber();

		mResults->AddFrame(frame);
		mResults->CommitResults();

		ReportProgress(frame.mEndingSampleInclusive);
	}
}

bool NANDFlashAnalyzer::NeedsRerun()
{
	return false;
}

U32 NANDFlashAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if (mSimulationInitilized == false)
	{
		mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate, simulation_channels);
}

U32 NANDFlashAnalyzer::GetMinimumSampleRateHz()
{
	return 10000;	// Unsure of the minimum; depends on the implementation; return the lowest rate.
}

const char* NANDFlashAnalyzer::GetAnalyzerName() const
{
	return "NAND Flash";
}

const char* GetAnalyzerName()
{
	return "NAND Flash";
}

Analyzer* CreateAnalyzer()
{
	return new NANDFlashAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}