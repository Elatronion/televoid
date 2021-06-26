#include <stdio.h>
#include <string.h>

typedef struct {
	const char* title;
	const char* name;
} credit_t;

#define CREDIT(title, name) (credit_t) { title, name }
#define CREDITS_LENGTH(credits) sizeof(credits)/sizeof(credit_t)

typedef struct {
	const char* title;
	const char* link;
	const char* license;
} soundtrack_attribution_t;

#define SOUNDTRACK_ATTRIBUTION(title, link, license) (soundtrack_attribution_t) { title, link, license }
#define SOUNDTRACK_ATTRIBUTIONS_LENGTH(soundtrack_attributions) sizeof(soundtrack_attributions)/sizeof(soundtrack_attribution_t)

int main() {
	credit_t credits[] = {
		CREDIT("'Invaders' Minigame, Windows Testing", "Alexis.M (Katsuro)"),
		CREDIT("Story & Concepts, Character Portrait Art, Dialogue Editor, Motivator", "Ari.S (Pearofthestars)"),
		CREDIT("Story & Concepts", "Cecelia.M"),
		CREDIT("Story & Concepts, Project Manager, Programming, Scene Creation, 3D Modeling, Minigame Creation, Game Engineer", "Ezra.H (Elatronion)"),
		CREDIT("Story & Concepts, Motivator", "Felix.D"),
		CREDIT("Story & Concepts, Background Concept Art", "Keaton.H"),
		CREDIT("Story & Concepts, 'SAM' Character Portrait Art, Windows Testing, Motivator", "Kitpigeon"),
		CREDIT("Story & Concepts", "Levi.M"),
		CREDIT("Radio ('radio.png') Art Resource", "Mr.Monocle"),
		CREDIT("Story & Concepts, Art, Scene Creation, Windows Testing, Local Frenchman, Motivator", "Normal Canard"),
		CREDIT("Additional Programming, Windows Building & Testing", "Thomas.L"),
		CREDIT("Story & Concepts", "Watson.J"),
		CREDIT("Outervoid Sounds", "Yann.H (Rad Tikal)")
	};

	soundtrack_attribution_t soundtrack_attributions[] = {
		SOUNDTRACK_ATTRIBUTION
		(
			"cheesy by Ian MacLeod (brutalmoose)",
			"https://soundcloud.com/brutalmoose/cheesy",
			"Show him some love!"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"door by Ian MacLeod (brutalmoose)",
			"https://soundcloud.com/brutalmoose/door",
			"Show him some love!"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"gone by Ian MacLeod (brutalmoose)",
			"https://soundcloud.com/brutalmoose/gone",
			"Show him some love!"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"maze (wip) by Ian MacLeod (brutalmoose)",
			"https://soundcloud.com/brutalmoose/maze-wip",
			"Show him some love!"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"Moose by nokibient",
			"https://soundcloud.com/nokbient/moose",
			"Thank you for allowing us to use your awesome song!"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"our heavenly bodies by Ian MacLeod (brutalmoose)",
			"https://soundcloud.com/brutalmoose/our-heavenly-bodies",
			"Show him some love!"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"sad again (wip) by Ian MacLeod (brutalmoose)",
			"https://soundcloud.com/brutalmoose/sad-again-wip",
			"Show him some love!"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"spaces by Ian MacLeod (brutalmoose)",
			"https://soundcloud.com/brutalmoose/spaces",
			"Show him some love!"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"spooky (wip) by Ian MacLeod (brutalmoose)",
			"https://soundcloud.com/brutalmoose/spooky-wip",
			"Show him some love!"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"sweet dreams by Ian MacLeod (brutalmoose)",
			"https://soundcloud.com/brutalmoose/sweet-dreams",
			"Show him some love!"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"Too Cool by Kevin MacLeod",
			"Link: https://incompetech.filmmusic.io/song/4534-too-cool",
			"License: https://filmmusic.io/standard-license"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"Townie Loop by Kevin MacLeod",
			"Link: https://incompetech.filmmusic.io/song/4541-townie-loop",
			"License: https://filmmusic.io/standard-license"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"Tree Of Life by nokibient",
			"https://soundcloud.com/nokbient/tree-of-life",
			"Thank you for allowing us to use your awesome song!"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"warmup by Ian MacLeod (brutalmoose)",
			"https://soundcloud.com/brutalmoose/warmup",
			"Show him some love!"
		),
		SOUNDTRACK_ATTRIBUTION
		(
			"'Zazie' Kevin MacLeod (incompetech.com)",
			"Licensed under Creative Commons: By Attribution 4.0 License",
			"http://creativecommons.org/licenses/by/4.0/"
		),
		SOUNDTRACK_ATTRIBUTION
		(
		 "",
		 "Thank YOU for playing!",
		 "Created using the Hyper Game Engine"
		)
	};

	// printf("Number of Credits : %d\n", CREDITS_LENGTH(credits));

	int end_timestamp = 1000;

	char string[255] = "Hello, World!";
	int timestamp = 0;
	int timestep = 5;

	printf("### Titles ###\nt 0 0 0 0.5 \"CREDITS\"\n# Set Position\nk linear x 0 0\nk linear y 0 0\nk linear x 1 0\nk linear y 1 35\n\n");

	int credits_length = CREDITS_LENGTH(credits);
	int soundtrack_attributions_length = SOUNDTRACK_ATTRIBUTIONS_LENGTH(soundtrack_attributions);
	int i;
	// Titles
	for(i = 0; i < credits_length; i++) {
		credit_t credit = credits[i];

		timestamp += timestep;
		strcpy(string, credit.title);
		printf("d %d \"%s\"\n", timestamp, string);
	}
	int soundtrack_attributions_start_timestamp = timestamp;
	for(i = 0; i < soundtrack_attributions_length ; i++) {
		soundtrack_attribution_t soundtrack_attribution = soundtrack_attributions[i];

		timestamp += timestep;
		strcpy(string, soundtrack_attribution.title);
		printf("d %d \"%s\"\n", timestamp, string);
	}
	printf("d %d \"FIN\"\n\n", end_timestamp);

	// Link
	printf("### Links ###\nt 0 0 0 0.5 \"\"\n\n");
	timestamp = soundtrack_attributions_start_timestamp;
	for(i = 0; i < soundtrack_attributions_length ; i++) {
		soundtrack_attribution_t soundtrack_attribution = soundtrack_attributions[i];

		timestamp += timestep;
		strcpy(string, soundtrack_attribution.link);
		printf("d %d \"%s\"\n", timestamp, string);
	}
	printf("d %d \"FIN\"\n\n", end_timestamp);

	// People / Licenses
	timestamp = 0;
	printf("### People / Licenses ###\nt 0 0 0 1 \"Alphabetically\"\n# Set Position\nk linear x 0 0\nk linear y 0 0\nk linear x 1 0\nk linear y 1 -35\n# Set Fontsize\nk hold sy 0 1\n\n# Make Text Yellow\nk linear colorR 0 0\nk linear colorG 0 0\nk linear colorB 0 0\nk linear colorA 0 0\nk linear colorR 1 0.996078431373\nk linear colorG 1 0.8\nk linear colorB 1 0.043137254902\n\n");
	for(i = 0; i < credits_length; i++) {
		credit_t credit = credits[i];

		timestamp += timestep;
		strcpy(string, credit.name);
		printf("d %d \"%s\"\n", timestamp, string);
	}

	printf("k hold sy %d 0.5\n", timestamp+timestep);

	for(i = 0; i < soundtrack_attributions_length ; i++) {
		soundtrack_attribution_t soundtrack_attribution = soundtrack_attributions[i];

		timestamp += timestep;
		strcpy(string, soundtrack_attribution.license);
		printf("d %d \"%s\"\n", timestamp, string);
	}

	printf("d %d \"FIN\"\n", end_timestamp);
	printf("k hold sy %d 0.5\n", end_timestamp);






	return 0;
}
