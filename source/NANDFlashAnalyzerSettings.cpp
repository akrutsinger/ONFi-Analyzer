#include "NANDFlashAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


NANDFlashAnalyzerSettings::NANDFlashAnalyzerSettings()
	: mIO0Channel(UNDEFINED_CHANNEL),
	mIO1Channel(UNDEFINED_CHANNEL),
	mIO2Channel(UNDEFINED_CHANNEL),
	mIO3Channel(UNDEFINED_CHANNEL),
	mIO4Channel(UNDEFINED_CHANNEL),
	mIO5Channel(UNDEFINED_CHANNEL),
	mIO6Channel(UNDEFINED_CHANNEL),
	mIO7Channel(UNDEFINED_CHANNEL),
	mWriteEnableChannel(UNDEFINED_CHANNEL),
	mReadEnableChannel(UNDEFINED_CHANNEL)
{
	mIO0Interface.reset(new AnalyzerSettingInterfaceChannel());
	mIO0Interface->SetTitleAndTooltip("IO 0",  "I/O Channel 0");
	mIO0Interface->SetChannel(mIO0Channel);
	mIO0Interface->SetSelectionOfNoneIsAllowed(false);

	mIO1Interface.reset(new AnalyzerSettingInterfaceChannel());
	mIO1Interface->SetTitleAndTooltip("IO 1", "I/O Channel 1");
	mIO1Interface->SetChannel(mIO1Channel);
	mIO1Interface->SetSelectionOfNoneIsAllowed(false);

	mIO2Interface.reset(new AnalyzerSettingInterfaceChannel());
	mIO2Interface->SetTitleAndTooltip("IO 2", "I/O Channel 2");
	mIO2Interface->SetChannel(mIO2Channel);
	mIO2Interface->SetSelectionOfNoneIsAllowed(false);

	mIO3Interface.reset(new AnalyzerSettingInterfaceChannel());
	mIO3Interface->SetTitleAndTooltip("IO 3", "I/O Channel 3");
	mIO3Interface->SetChannel(mIO3Channel);
	mIO3Interface->SetSelectionOfNoneIsAllowed(false);

	mIO4Interface.reset(new AnalyzerSettingInterfaceChannel());
	mIO4Interface->SetTitleAndTooltip("IO 4", "I/O Channel 4");
	mIO4Interface->SetChannel(mIO4Channel);
	mIO4Interface->SetSelectionOfNoneIsAllowed(false);

	mIO5Interface.reset(new AnalyzerSettingInterfaceChannel());
	mIO5Interface->SetTitleAndTooltip("IO 5", "I/O Channel 5");
	mIO5Interface->SetChannel(mIO5Channel);
	mIO5Interface->SetSelectionOfNoneIsAllowed(false);

	mIO6Interface.reset(new AnalyzerSettingInterfaceChannel());
	mIO6Interface->SetTitleAndTooltip("IO 6", "I/O Channel 6");
	mIO6Interface->SetChannel(mIO6Channel);
	mIO6Interface->SetSelectionOfNoneIsAllowed(false);

	mIO7Interface.reset(new AnalyzerSettingInterfaceChannel());
	mIO7Interface->SetTitleAndTooltip("IO 7", "I/O Channel 7");
	mIO7Interface->SetChannel(mIO7Channel);
	mIO7Interface->SetSelectionOfNoneIsAllowed(false);

	mWriteEnableInterface.reset(new AnalyzerSettingInterfaceChannel());
	mWriteEnableInterface->SetTitleAndTooltip("WriteEnable", "Write Enable line (WE#)");
	mWriteEnableInterface->SetChannel(mWriteEnableChannel);
	mWriteEnableInterface->SetSelectionOfNoneIsAllowed(false);

	mReadEnableInterface.reset(new AnalyzerSettingInterfaceChannel());
	mReadEnableInterface->SetTitleAndTooltip("ReadEnable", "Read Enable line (RE#)");
	mReadEnableInterface->SetChannel(mReadEnableChannel);
	mReadEnableInterface->SetSelectionOfNoneIsAllowed(false);

	AddInterface(mIO0Interface.get());
	AddInterface(mIO1Interface.get());
	AddInterface(mIO2Interface.get());
	AddInterface(mIO3Interface.get());
	AddInterface(mIO4Interface.get());
	AddInterface(mIO5Interface.get());
	AddInterface(mIO6Interface.get());
	AddInterface(mIO7Interface.get());
	AddInterface(mWriteEnableInterface.get());
	AddInterface(mReadEnableInterface.get());

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel(mIO0Channel, "I/O 0", false);
	AddChannel(mIO1Channel, "I/O 1", false);
	AddChannel(mIO2Channel, "I/O 2", false);
	AddChannel(mIO3Channel, "I/O 3", false);
	AddChannel(mIO4Channel, "I/O 4", false);
	AddChannel(mIO5Channel, "I/O 5", false);
	AddChannel(mIO6Channel, "I/O 6", false);
	AddChannel(mIO7Channel, "I/O 7", false);
	AddChannel(mWriteEnableChannel, "WriteEnable", false);
	AddChannel(mReadEnableChannel, "ReadEnable", false);
}

NANDFlashAnalyzerSettings::~NANDFlashAnalyzerSettings()
{
}

bool NANDFlashAnalyzerSettings::SetSettingsFromInterfaces()
{
	/* Ensure channels do not overlap */
	Channel io0 = mIO0Interface->GetChannel();
	Channel io1 = mIO1Interface->GetChannel();
	Channel io2 = mIO2Interface->GetChannel();
	Channel io3 = mIO3Interface->GetChannel();
	Channel io4 = mIO4Interface->GetChannel();
	Channel io5 = mIO5Interface->GetChannel();
	Channel io6 = mIO6Interface->GetChannel();
	Channel io7 = mIO7Interface->GetChannel();
	Channel write_enable = mWriteEnableInterface->GetChannel();
	Channel read_enable = mReadEnableInterface->GetChannel();

	std::vector<Channel> channels;
	channels.push_back(io0);
	channels.push_back(io1);
	channels.push_back(io2);
	channels.push_back(io3);
	channels.push_back(io4);
	channels.push_back(io5);
	channels.push_back(io6);
	channels.push_back(io7);
	channels.push_back(write_enable);
	channels.push_back(read_enable);

	if (AnalyzerHelpers::DoChannelsOverlap(&channels[0], channels.size()) == true)
	{
		SetErrorText("Please select different channels for each I/O or Read/Write lines.");
		return false;
	}

	if ((io0 == UNDEFINED_CHANNEL)
		|| (io1 == UNDEFINED_CHANNEL)
		|| (io1 == UNDEFINED_CHANNEL)
		|| (io2 == UNDEFINED_CHANNEL)
		|| (io3 == UNDEFINED_CHANNEL)
		|| (io4 == UNDEFINED_CHANNEL)
		|| (io5 == UNDEFINED_CHANNEL)
		|| (io6 == UNDEFINED_CHANNEL)
		|| (io7 == UNDEFINED_CHANNEL)
		|| (write_enable == UNDEFINED_CHANNEL)
		|| (read_enable == UNDEFINED_CHANNEL))
	{
		SetErrorText("Please select an input for all I/O lines, Read Enable, and Write Enable.");
		return false;
	}

	mIO0Channel = mIO0Interface->GetChannel();
	mIO1Channel = mIO1Interface->GetChannel();
	mIO2Channel = mIO2Interface->GetChannel();
	mIO3Channel = mIO3Interface->GetChannel();
	mIO4Channel = mIO4Interface->GetChannel();
	mIO5Channel = mIO5Interface->GetChannel();
	mIO6Channel = mIO6Interface->GetChannel();
	mIO7Channel = mIO7Interface->GetChannel();
	mWriteEnableChannel = mWriteEnableInterface->GetChannel();
	mReadEnableChannel = mReadEnableInterface->GetChannel();

	ClearChannels();
	AddChannel(mIO0Channel, "I/O 0", mIO0Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO1Channel, "I/O 1", mIO1Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO2Channel, "I/O 2", mIO2Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO3Channel, "I/O 3", mIO3Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO4Channel, "I/O 4", mIO4Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO5Channel, "I/O 5", mIO5Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO6Channel, "I/O 6", mIO6Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO7Channel, "I/O 7", mIO7Channel != UNDEFINED_CHANNEL);
	AddChannel(mWriteEnableChannel, "WriteEnable", mWriteEnableChannel != UNDEFINED_CHANNEL);
	AddChannel(mReadEnableChannel, "ReadEnable", mReadEnableChannel != UNDEFINED_CHANNEL);

	return true;
}

void NANDFlashAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mIO0Interface->SetChannel(mIO0Channel);
	mIO1Interface->SetChannel(mIO1Channel);
	mIO2Interface->SetChannel(mIO2Channel);
	mIO3Interface->SetChannel(mIO3Channel);
	mIO4Interface->SetChannel(mIO4Channel);
	mIO5Interface->SetChannel(mIO5Channel);
	mIO6Interface->SetChannel(mIO6Channel);
	mIO7Interface->SetChannel(mIO7Channel);
	mWriteEnableInterface->SetChannel(mWriteEnableChannel);
	mReadEnableInterface->SetChannel(mReadEnableChannel);
}

void NANDFlashAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString(settings);

	text_archive >> mIO1Channel;
	text_archive >> mIO2Channel;
	text_archive >> mIO3Channel;
	text_archive >> mIO4Channel;
	text_archive >> mIO5Channel;
	text_archive >> mIO6Channel;
	text_archive >> mIO7Channel;
	text_archive >> mWriteEnableChannel;
	text_archive >> mReadEnableChannel;

	ClearChannels();
	AddChannel(mIO0Channel, "I/O 0", mIO0Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO1Channel, "I/O 1", mIO1Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO2Channel, "I/O 2", mIO2Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO3Channel, "I/O 3", mIO3Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO4Channel, "I/O 4", mIO4Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO5Channel, "I/O 5", mIO5Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO6Channel, "I/O 6", mIO6Channel != UNDEFINED_CHANNEL);
	AddChannel(mIO7Channel, "I/O 7", mIO7Channel != UNDEFINED_CHANNEL);
	AddChannel(mWriteEnableChannel, "WriteEnable", mWriteEnableChannel != UNDEFINED_CHANNEL);
	AddChannel(mReadEnableChannel, "ReadEnable", mReadEnableChannel != UNDEFINED_CHANNEL);

	UpdateInterfacesFromSettings();
}

const char* NANDFlashAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mIO1Channel;
	text_archive << mIO2Channel;
	text_archive << mIO3Channel;
	text_archive << mIO4Channel;
	text_archive << mIO5Channel;
	text_archive << mIO6Channel;
	text_archive << mIO7Channel;
	text_archive << mWriteEnableChannel;
	text_archive << mReadEnableChannel;

	return SetReturnString(text_archive.GetString());
}
