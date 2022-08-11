#ifndef NANDFLASH_ANALYZER_SETTINGS
#define NANDFLASH_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

enum NANDFlashIOOperation { NAND_READ, NAND_WRITE };
enum NANDFlashCommand {

};

class NANDFlashAnalyzerSettings : public AnalyzerSettings
{
public:
	NANDFlashAnalyzerSettings();
	virtual ~NANDFlashAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	Channel mIO0Channel;
	Channel mIO1Channel;
	Channel mIO2Channel;
	Channel mIO3Channel;
	Channel mIO4Channel;
	Channel mIO5Channel;
	Channel mIO6Channel;
	Channel mIO7Channel;
	Channel mWriteEnableChannel;
	Channel mReadEnableChannel;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mIO0Interface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mIO1Interface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mIO2Interface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mIO3Interface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mIO4Interface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mIO5Interface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mIO6Interface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mIO7Interface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mWriteEnableInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mReadEnableInterface;
};

#endif //NANDFLASH_ANALYZER_SETTINGS
