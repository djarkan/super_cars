#ifndef __ASSETKEYS__
#define __ASSETKEYS__

namespace textures
{
	enum ID
	{
	    loading,
	    gremlins,
	    magnetic,
	    super_cars,
	    guyAndGirl,
	    ronAndNancy,
	    credits_original,
	    credits_additionnal,
	    pseudo,
	    game,
		cars,
		spritesheet,
		track,
		shadows,
        roadUp,
        roadDown,
        office,
        garage,
        darkgarage,
        ronAndNancyFont,
        supercarsfont,
        garageFont,
        goldenFont,
        whitecar,
        bluecar,
        redcar
	};
}

namespace sounds
{
	enum ID
	{
		motor,
		carsbump,
		countdowninter,
		countdownfinal,
		tyres1,
		tyres2,
		applause,
		gameover,
		sold
	};
}

namespace json
{
	enum ID
	{
		config,
		language
	};
}

#endif // __ASSETKEYS__
