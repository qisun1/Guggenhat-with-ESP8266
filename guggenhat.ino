#define FASTLED_ALLOW_INTERRUPTS 0

#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontMatrise.h>

#include <LEDSprites.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//define fastled setting
#define LED_PIN        D3
#define COLOR_ORDER    GRB
#define CHIPSET     WS2812B

#define X_EDGE SPRITE_EDGE_X_MIN|SPRITE_EDGE_X_MAX

//Matrix definition
char br = '1'; //default brightness scale, lowest, 1 to 5, 5 the most bright
uint8_t brightness = 10; //default brightness value
#define MATRIX_TYPE    HORIZONTAL_MATRIX
#define MATRIX_WIDTH   37
#define MATRIX_HEIGHT  -8  //for ledtext matrix, 

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds; //canvas for text
char SpriteOrText = '1';

//for LEDtext
#define LEADSPACE 4
#define GAPSPACE 2
#define TotalMessageBytes 150 //including special characters and leading,gap spaces
char SpeedLevel = '3';  // this is for default scrolling speed, 1 to 5, 1 for slowest
char Color = '5'; //text color
uint16_t speed = 90;
cLEDText ScrollingMsg;
unsigned char TxtMsg[TotalMessageBytes] = { EFFECT_SCROLL_LEFT "    " };
uint8_t TxtLen;

//for sprite
cLEDSprites Sprites(&leds);
cSprite currentsprite[5];
char SpriteType = '1'; //#1 flag; #2 animation
char SpriteId; 
#define flagspeed 300
const uint8_t SpriteFish1Data[] = {
	//frame 1
	B8_2BIT(10111000),
	B8_2BIT(01121000),
	B8_2BIT(10111000),
	//frame 2
	B8_2BIT(11101000),
	B8_2BIT(12110000),
	B8_2BIT(11101000)
};
const uint8_t SpriteFish1Mask[] = {
	//frame 1
	B8_1BIT(10111000),
	B8_1BIT(01101000),
	B8_1BIT(10111000),
	//frame 2
	B8_1BIT(11101000),
	B8_1BIT(10110000),
	B8_1BIT(11101000)
};
const struct CRGB SpriteFish1Cols[3] = { CRGB(187,37,130), CRGB(0,0,100), CRGB(0,0,0) };


const uint8_t Spritefish2Data[] = {
	//frame 1
	B8_2BIT(00000001),
	B8_2BIT(01111010),
	B8_2BIT(12111110),
	B8_2BIT(01111010),
	B8_2BIT(00000001),
	//frame 2
	B8_2BIT(10000000),
	B8_2BIT(01011110),
	B8_2BIT(01111121),
	B8_2BIT(01011110),
	B8_2BIT(10000000)
};
const uint8_t Spritefish2Mask[] = {
	B8_1BIT(11111110),
	B8_1BIT(11111110),
	B8_1BIT(11111111),
	B8_1BIT(11111110),
	B8_1BIT(11111110),
	//frame2
	B8_1BIT(11111110),
	B8_1BIT(11111110),
	B8_1BIT(11111111),
	B8_1BIT(11111110),
	B8_1BIT(11111110),

};
const struct CRGB Spritefish2Cols[3] = { CRGB(255,235,0), CRGB(255,24,114), CRGB(0,0,0) };

const uint8_t SpritejellyData[] = {
	//frame 1
	B8_1BIT(01111000),
	B8_1BIT(01111000),
	B8_1BIT(00110000),
	B8_1BIT(00110000),
	B8_1BIT(00110000),
	B8_1BIT(00000000),
	B8_1BIT(00000000),
	B8_1BIT(00000000),
	//frame 2
	B8_1BIT(01111000),
	B8_1BIT(10110100),
	B8_1BIT(00110000),
	B8_1BIT(00110000),
	B8_1BIT(00110000),
	B8_1BIT(00000000),
	B8_1BIT(00000000),
	B8_1BIT(00000000),
	//frame 3
	B8_1BIT(00000000),
	B8_1BIT(01111000),
	B8_1BIT(10110100),
	B8_1BIT(00110000),
	B8_1BIT(00110000),
	B8_1BIT(00110000),
	B8_1BIT(00000000),
	B8_1BIT(00000000),
	//frame 4
	B8_1BIT(00000000),
	B8_1BIT(00000000),
	B8_1BIT(01111000),
	B8_1BIT(10110100),
	B8_1BIT(00110000),
	B8_1BIT(00110000),
	B8_1BIT(01001000),
	B8_1BIT(00000000),
	//frame 5
	B8_1BIT(00000000),
	B8_1BIT(00000000),
	B8_1BIT(00000000),
	B8_1BIT(01111000),
	B8_1BIT(10110100),
	B8_1BIT(00110000),
	B8_1BIT(01001000),
	B8_1BIT(01001000),
	//frame 6
	B8_1BIT(00000000),
	B8_1BIT(00000000),
	B8_1BIT(00000000),
	B8_1BIT(01111000),
	B8_1BIT(01111000),
	B8_1BIT(00110000),
	B8_1BIT(00110000),
	B8_1BIT(00110000),
	//frame 7
	B8_1BIT(00000000),
	B8_1BIT(01111000),
	B8_1BIT(01111000),
	B8_1BIT(00110000),
	B8_1BIT(00110000),
	B8_1BIT(00110000),
	B8_1BIT(00000000),
	B8_1BIT(00000000),
};
const struct CRGB SpritejellyCols[1] = { CRGB(155,247,255) };


const uint8_t BubbleData[] = {
	//frame 1
	B8_1BIT(10000000)
};
const struct CRGB BubbleColor[1] = { CRGB(100,100,100)};


const uint8_t ShapeData2[] =
{
	B8_1BIT(00110000),
	B8_1BIT(01111000),
	B8_1BIT(11111100),
	B8_1BIT(01111000),
	B8_1BIT(00110000),

};
struct CRGB ColTable2[1] = { CRGB(0xff, 0x19, 0x00) };

#define RAINBOW_WIDTH    16
#define RAINBOW_HEIGHT   8
const uint8_t RainbowData[] =
{
	B8_3BIT(01111111),B8_3BIT(11111110),
	B8_3BIT(11222222),B8_3BIT(22222211),
	B8_3BIT(12233333),B8_3BIT(33333221),
	B8_3BIT(12334444),B8_3BIT(44443321),
	B8_3BIT(12344555),B8_3BIT(55544321),
	B8_3BIT(12345566),B8_3BIT(66554321),
	B8_3BIT(12345667),B8_3BIT(76654321),
	B8_3BIT(12345677),B8_3BIT(77654321),
};
const uint8_t RainbowMask[] =
{
	B8_1BIT(01111111),B8_1BIT(11111110),
	B8_1BIT(11111111),B8_1BIT(11111111),
	B8_1BIT(11111111),B8_1BIT(11111111),
	B8_1BIT(11111111),B8_1BIT(11111111),
	B8_1BIT(11111111),B8_1BIT(11111111),
	B8_1BIT(11111111),B8_1BIT(11111111),
	B8_1BIT(11111111),B8_1BIT(11111111),
	B8_1BIT(11111111),B8_1BIT(11111111),
};
struct CRGB RainbowColTable[7] = { CHSV(216, 255, 255), CHSV(180, 255, 255), CHSV(144, 255, 255), CHSV(108, 255, 255), CHSV(72, 255, 255), CHSV(36, 255, 255), CHSV(0, 255, 255) };

const uint8_t FlagMask[] =
{
	B8_1BIT(01111111),B8_1BIT(11111110),
	B8_1BIT(11111111),B8_1BIT(11111111),
	B8_1BIT(11111111),B8_1BIT(11111111),
	B8_1BIT(11111111),B8_1BIT(11111111),
	B8_1BIT(11111111),B8_1BIT(11111111),
	B8_1BIT(11111111),B8_1BIT(11111111),
	B8_1BIT(11111111),B8_1BIT(11111111),
	B8_1BIT(11111111),B8_1BIT(11111111),
};

//us flag
const uint8_t USFlag[] = {
	B8_2BIT(12121213),B8_2BIT(33333330),
	B8_2BIT(21212122),B8_2BIT(22222220),
	B8_2BIT(12121213),B8_2BIT(33333330),
	B8_2BIT(21212122),B8_2BIT(22222220),
	B8_2BIT(12121213),B8_2BIT(33333330),
	B8_2BIT(22222222),B8_2BIT(22222220),
	B8_2BIT(33333333),B8_2BIT(33333330),
	B8_2BIT(22222222),B8_2BIT(22222220) };
const struct CRGB SpriteusCols[3] = { CRGB(0,40,104), CRGB(255,255,255), CRGB(191,10,48) };

//flag china
const uint8_t ChinaFlag[] = {
	B8_2BIT(11111111),B8_2BIT(11111110),
	B8_2BIT(11211121),B8_2BIT(11111110),
	B8_2BIT(12221111),B8_2BIT(11111110),
	B8_2BIT(12121112),B8_2BIT(11111110),
	B8_2BIT(11111111),B8_2BIT(11111110),
	B8_2BIT(12112111),B8_2BIT(11111110),
	B8_2BIT(11111111),B8_2BIT(11111110),
	B8_2BIT(11111111),B8_2BIT(11111110) };
const struct CRGB SpritechinaCols[3] = { CRGB(224,21,0), CRGB(255,212,0), CRGB(0,0,0) };

const uint8_t FranceFlag[] = {
	B8_2BIT(11111222),B8_2BIT(22333330),
	B8_2BIT(11111222),B8_2BIT(22333330),
	B8_2BIT(11111222),B8_2BIT(22333330),
	B8_2BIT(11111222),B8_2BIT(22333330),
	B8_2BIT(11111222),B8_2BIT(22333330),
	B8_2BIT(11111222),B8_2BIT(22333330),
	B8_2BIT(11111222),B8_2BIT(22333330),
	B8_2BIT(11111222),B8_2BIT(22333330) };
const struct CRGB SpritefranceCols[3] = { CRGB(0,0,255), CRGB(255,255,255), CRGB(255,0,0) };

const uint8_t GermanyFlag[] = {
	B8_2BIT(00000000),B8_2BIT(00000000),
	B8_2BIT(00000000),B8_2BIT(00000000),
	B8_2BIT(11111111),B8_2BIT(11111110),
	B8_2BIT(11111111),B8_2BIT(11111110),
	B8_2BIT(11111111),B8_2BIT(11111110),
	B8_2BIT(22222222),B8_2BIT(22222220),
	B8_2BIT(22222222),B8_2BIT(22222220),
	B8_2BIT(22222222),B8_2BIT(22222220) };
const struct CRGB SpritegermanyCols[3] = { CRGB(208,0,0), CRGB(255,206,0), CRGB(0,0,0) };

const uint8_t PolandFlag[] = {
	B8_2BIT(11111111),B8_2BIT(11111110),
	B8_2BIT(11111111),B8_2BIT(11111110),
	B8_2BIT(11111111),B8_2BIT(11111110),
	B8_2BIT(11111111),B8_2BIT(11111110),
	B8_2BIT(22222222),B8_2BIT(22222220),
	B8_2BIT(22222222),B8_2BIT(22222220),
	B8_2BIT(22222222),B8_2BIT(22222220),
	B8_2BIT(22222222),B8_2BIT(22222220) };
const struct CRGB SpritepolandCols[3] = { CRGB(255,255,255), CRGB(220,20,60), CRGB(0,0,0) };

const uint8_t UKFlag[] = {
	B8_2BIT(12321123),B8_2BIT(21123210),
	B8_2BIT(11232123),B8_2BIT(21232110),
	B8_2BIT(22223223),B8_2BIT(22322220),
	B8_2BIT(33333333),B8_2BIT(33333330),
	B8_2BIT(22223223),B8_2BIT(22322220),
	B8_2BIT(11132123),B8_2BIT(21232110),
	B8_2BIT(12321123),B8_2BIT(21123210),
	B8_2BIT(23211123),B8_2BIT(21112320) };
const struct CRGB SpriteukCols[3] = { CRGB(0,51,153), CRGB(255,255,241), CRGB(204,0,0) };

const uint8_t JapanFlag[] = {
	B8_2BIT(11111111),B8_2BIT(11111110),
	B8_2BIT(11111122),B8_2BIT(21111110),
	B8_2BIT(11111222),B8_2BIT(22111110),
	B8_2BIT(11112222),B8_2BIT(22211110),
	B8_2BIT(11112222),B8_2BIT(22211110),
	B8_2BIT(11111222),B8_2BIT(22111110),
	B8_2BIT(11111122),B8_2BIT(21111110),
	B8_2BIT(11111111),B8_2BIT(11111110) };
const struct CRGB SpritejapanCols[3] = { CRGB(255,255,255), CRGB(199,0,37), CRGB(0,0,0) };

const uint8_t SpritekoreaData[] = {
	B8_2BIT(11111111),B8_2BIT(11111110),
	B8_2BIT(11100111),B8_2BIT(11001110),
	B8_2BIT(11001122),B8_2BIT(21100110),
	B8_2BIT(11111222),B8_2BIT(32111110),
	B8_2BIT(11111323),B8_2BIT(33111110),
	B8_2BIT(11001133),B8_2BIT(31100110),
	B8_2BIT(11100111),B8_2BIT(11001110),
	B8_2BIT(11111111),B8_2BIT(11111110) };
const struct CRGB SpritekoreaCols[3] = { CRGB(255,255,255), CRGB(198,12,48), CRGB(0,52,120) };



//for wifi
//SSID and Password to your ESP Access Point
const char ssid[] = "myhat2";
const char password[] = "12345678";
ESP8266WebServer server(80); //Server on port 80

//web page
const String pageheader = "<html><head><title>My Hat</title></head><meta name=viewport content = \"width=device-width; initial-scale=1.0\" / >"
"<script>function ccolor(t){document.getElementById('cc').value=t;}</script>"
"<body><h1>My Hat</h1><br><form action='/' method='POST'>";

const String colortable = "<table border='1' width='300' height='80'><tr><td bgcolor='white' onclick='ccolor(1);'>"
"</td><td bgcolor='#b31b1b' onclick='ccolor(2);'>"
"</td><td bgcolor='blue' onclick='ccolor(3);'>"
"</td><td bgcolor='green' onclick='ccolor(4);'>"
"</td><td bgcolor='purple' onclick='ccolor(5);'>"
"</tr><tr><td bgcolor = 'yellow' onclick = 'ccolor(6);'>"
"</td><td bgcolor='orange' onclick='ccolor(7);'>"
"</td><td bgcolor='#7FFF00' onclick='ccolor(8);'>"
"</td><td bgcolor='aqua' onclick='ccolor(9);'>"
"</td><td bgcolor='#228B22' onclick='ccolor(0);'>"
"</td></tr></table><br>";
							 //===============================================================
							 //                  SETUP
							 //===============================================================
void setup(void) {
	Serial.begin(115200);
	Serial.println("");
	WiFi.mode(WIFI_AP);           //Only Access point
	WiFi.softAP(ssid, password);
	IPAddress myIP = WiFi.softAPIP(); //Get IP address
	Serial.print("HotSpt IP:");
	Serial.println(myIP);

	server.on("/", handleRoot);      //Which routine to handle at root location

	server.begin();                  //Start server
	Serial.println("HTTP server started");

	FastLED.clear(true);
	FastLED.show();
	FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());
	FastLED.setBrightness(brightness);
	FastLED.clear(true);

	ScrollingMsg.SetFont(MatriseFontData);
	ScrollingMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 1, 0, 1);
	//ScrollingMsg.SetText((unsigned char *)TxtDemo, sizeof(TxtDemo) - 1);
	ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0xff, 0x00, 0xff);
}

//===============================================================
//                     LOOP
//===============================================================
void loop(void) {
	server.handleClient();          //Handle client requests
	FastLED.setBrightness(brightness);
	if (SpriteOrText == '1')
	{
		if (ScrollingMsg.UpdateText() == -1)
			ScrollingMsg.SetText((unsigned char *)TxtMsg, TxtLen);
		else
			FastLED.show();
	}
	else
	{
		FastLED.clear();
		Sprites.UpdateSprites();

		if (SpriteType == '2')
		{
			if (SpriteId == '1')
			{
				uint8_t t1 = currentsprite[0].GetFlags();
				if (t1 & (SPRITE_EDGE_X_MIN))
				{
					currentsprite[0].SetFrame(0, 0);
				}
				else if (t1 & (SPRITE_EDGE_X_MAX))
				{
					currentsprite[0].SetFrame(1, 0);
				}

				t1 = currentsprite[1].GetFlags();
				if (t1 & (SPRITE_EDGE_X_MIN))
				{
					currentsprite[1].SetFrame(1, 0);
				}
				else if (t1 & (SPRITE_EDGE_X_MAX))
				{
					currentsprite[1].SetFrame(0, 0);
				}

				t1 = currentsprite[2].GetFlags();
				if (t1 & (SPRITE_MATRIX_Y_OFF))
				{
					uint8_t xp = random8(3, 30);
					Serial.println("reset bubble 1");
					currentsprite[2].SetPosition(xp, 0);
				}

				t1 = currentsprite[3].GetFlags();
				if (t1 & (SPRITE_MATRIX_Y_OFF))
				{
					uint8_t xp = random8(3, 30);
					Serial.println("reset bubble 1");
					currentsprite[3].SetPosition(xp, 0);
				}

			}
			else if (SpriteId == '2')
			{

			}
		}

		Sprites.RenderSprites();
		FastLED.show();
	}
	//Serial.println(speed);
	delay(speed);
}


void SendPage(String myText) {
	String sp1 = "";
	String sp2 = "";
	String sp3 = "";
	String sp4 = "";
	String sp5 = "";

	String br1 = "";
	String br2 = "";
	String br3 = "";
	String br4 = "";
	String br5 = "";

	switch (SpeedLevel)
	{
	case '1': sp1 = "checked";  break;
	case '2': sp2 = "checked";  break;
	case '3': sp3 = "checked";  break;
	case '4': sp4 = "checked";  break;
	case '5': sp5 = "checked";  break;
	}

	switch (br)
	{
	case '1': br1 = "checked";   break;
	case '2': br2 = "checked";   break;
	case '3': br3 = "checked";  break;
	case '4': br4 = "checked";  break;
	case '5': br5 = "checked"; break;
	}

	//send web page
	server.sendHeader(server.header(200), "text/html");
	server.sendContent(pageheader);
	String page = "<b>Speed:</b> Fast<INPUT type='radio' name='LED' value='1' " + sp1 + " />";
	page += "<INPUT type='radio' name='SP' value='2' " + sp2 + " />";
	page += "<INPUT type='radio' name='SP' value='3' " + sp3 + " />";
	page += "<INPUT type='radio' name='SP' value='4' " + sp4 + " />";
	page += "<INPUT type='radio' name='SP' value='5' " + sp5 + " />Slow<br><br>";
	page += "<b>Brightness:</b> Low<INPUT type='radio' name='BR' value='1' " + br1 + " />";
	page += "<INPUT type='radio' name='BR' value='2' " + br2 + " />";
	page += "<INPUT type='radio' name='BR' value='3' " + br3 + " />";
	page += "<INPUT type='radio' name='BR' value='4' " + br4 + " />";
	page += "<INPUT type='radio' name='BR' value='5' " + br5 + " />High<br><br>";
	server.sendContent(page);
	server.sendContent(colortable);
	
	page = "<b>Text Color:</b> <input type = 'text' size='10' name ='cc' id='cc' value = '";
	page += Color;
	page += "'><br><br><b>Flags: </b>#1 US; #2 China; #3 France; #4 Poland; #5 Japan; #6 UK; #7 German; #8 Korea";
	page += "<br><b>Animations: </b>*1 Fish;<br><br>";
	page += "<b>Text: </b><INPUT type='text' name='name' maxlength='30' style='text-transform:uppercase' value='" + myText;
	page += "'><br><br><INPUT type='submit' value='Submit'></form></body></html>";
	server.sendContent(page);
}

void handleRoot() {
	String myText = "";
	if (server.hasArg("SP"))
	{
		SpeedLevel = server.arg("SP")[0];
		Serial.println(SpeedLevel);

		switch (SpeedLevel)
		{
		case '1':  speed = 70;  break;
		case '2':  speed = 80;  break;
		case '3':  speed = 100; break;
		case '4':  speed = 125; break;
		case '5':  speed = 140; break;
		}


	}

	if (server.hasArg("BR"))
	{
		br = server.arg("BR")[0];
		Serial.println(br);
		switch (br)
		{
		case '1':  brightness = 10;  break;
		case '2':  brightness = 20;  break;
		case '3':  brightness = 40; break;
		case '4':  brightness = 80; break;
		case '5':  brightness = 110; break;
		}

	}

	if (server.hasArg("cc"))
	{
		Color = server.arg("cc")[0];
		Serial.println(Color);
		CRGB textcolor = CRGB(0xd0, 0x00, 0xd0);
		switch (Color)
		{
		case '1': textcolor = CRGB(0x56, 0x56, 0x56);  break;
		case '2': textcolor = CRGB(0xf0, 0x17, 0x16);  break;
		case '3': textcolor = CRGB(0x00, 0x00, 0xff);  break;
		case '4': textcolor = CRGB(0x00, 0xff, 0x00);  break;
		case '5': textcolor = CRGB(0xd0, 0x00, 0xd0);  break;
		case '6': textcolor = CRGB(0xdd, 0x57, 0x00);  break;
		case '7': textcolor = CRGB(0xdd, 0x3f, 0x00);  break;
		case '8': textcolor = CRGB(0x17, 0x7a, 0x1f);  break;
		case '9': textcolor = CRGB(0x17, 0x7c, 0x76);  break;
		case '0': textcolor = CRGB(0x10c, 0x76, 0x16); break;
		}
		ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, textcolor.r, textcolor.g, textcolor.b);
	}

	if (server.hasArg("name"))
	{
		myText = server.arg("name");


		if ((myText[0] == '#') || (myText[0] == '*'))
		{
			SpriteOrText = '2';

			Sprites.RemoveAllSprites();
			FastLED.clear(true);
			FastLED.show();

			SpriteId = myText[1];

			if (myText[0] == '#')
			{
				SpriteType = '1';
				speed = flagspeed;
				switch (SpriteId)
				{
					case '1': currentsprite[0].Setup(15, 8, USFlag, 1, _2BIT, SpriteusCols, FlagMask); break;
					case '2': currentsprite[0].Setup(15, 8, ChinaFlag, 1, _2BIT, SpritechinaCols, FlagMask); break;
					case '3': currentsprite[0].Setup(15, 8, FranceFlag, 1, _2BIT, SpritefranceCols, FlagMask); break;
					case '4': currentsprite[0].Setup(15, 8, PolandFlag, 1, _2BIT, SpritepolandCols, FlagMask); break;
					case '5': currentsprite[0].Setup(15, 8, JapanFlag, 1, _2BIT, SpritejapanCols, FlagMask); break;
					case '6': currentsprite[0].Setup(15, 8, UKFlag, 1, _2BIT, SpriteukCols, FlagMask); break;
					case '7': currentsprite[0].Setup(15, 8, GermanyFlag, 1, _2BIT, SpritegermanyCols, FlagMask); break;
					case '8': currentsprite[0].Setup(15, 8, SpritekoreaData, 1, _2BIT, SpritekoreaCols, FlagMask); break;
					default:  currentsprite[0].Setup(15, 8, USFlag, 1, _2BIT, SpriteusCols, FlagMask); break;
				}
				currentsprite[0].SetPositionFrameMotionOptions(10/*X*/, 0/*Y*/, 0/*Frame*/, 0/*FrameRate*/, +1/*XChange*/, 1/*XRate*/, 0/*YChange*/, 0/*YRate*/, SPRITE_DETECT_EDGE | SPRITE_X_KEEPIN | SPRITE_Y_KEEPIN);
				Sprites.AddSprite(&currentsprite[0]);
			}
			else
			{
				SpriteType = '2';
				speed = 125;
				switch (SpriteId)
				{
					case '1': 
						currentsprite[4].Setup(6, 8, SpritejellyData, 7, _1BIT, SpritejellyCols, SpritejellyData);
						currentsprite[4].SetPositionFrameMotionOptions(15/*X*/, 0/*Y*/, 0/*Frame*/, 2/*FrameRate*/, 1/*XChange*/, 20/*XRate*/, 0/*YChange*/, 0/*YRate*/, SPRITE_DETECT_EDGE | SPRITE_X_KEEPIN);
						Sprites.AddSprite(&currentsprite[4]);

						currentsprite[0].Setup(5, 3, SpriteFish1Data, 2, _2BIT, SpriteFish1Cols, SpriteFish1Mask);
						currentsprite[0].SetPositionFrameMotionOptions(2/*X*/, 0/*Y*/, 0/*Frame*/, 0/*FrameRate*/, +1/*XChange*/, 2/*XRate*/, 1/*YChange*/, 2/*YRate*/, SPRITE_DETECT_EDGE | SPRITE_X_KEEPIN | SPRITE_Y_KEEPIN);
						Sprites.AddSprite(&currentsprite[0]);

						currentsprite[1].Setup(8, 5, Spritefish2Data, 2, _2BIT, Spritefish2Cols, Spritefish2Mask);
						currentsprite[1].SetPositionFrameMotionOptions(27/*X*/, 2/*Y*/, 0/*Frame*/, 0/*FrameRate*/, -1/*XChange*/, 2/*XRate*/, 0/*YChange*/, 0/*YRate*/, SPRITE_DETECT_EDGE | SPRITE_X_KEEPIN | SPRITE_Y_KEEPIN);
						Sprites.AddSprite(&currentsprite[1]);

						currentsprite[2].Setup(1, 1, BubbleData, 1, _1BIT, BubbleColor, BubbleData);
						currentsprite[2].SetPositionFrameMotionOptions(10/*X*/, 0/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 1/*YChange*/, 1/*YRate*/, SPRITE_DETECT_EDGE);
						Sprites.AddSprite(&currentsprite[2]);

						currentsprite[3].Setup(1, 1, BubbleData, 1, _1BIT, BubbleColor, BubbleData);
						currentsprite[3].SetPositionFrameMotionOptions(25/*X*/, 0/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 1/*YChange*/, 1/*YRate*/, SPRITE_DETECT_EDGE);
						Sprites.AddSprite(&currentsprite[2]);



						break;
					case '2':
						break;
					default: currentsprite[0].Setup(5, 3, SpriteFish1Data, 2, _2BIT, SpriteFish1Cols, SpriteFish1Mask); break;
							currentsprite[0].SetPositionFrameMotionOptions(2/*X*/, 0/*Y*/, 0/*Frame*/, 0/*FrameRate*/, +1/*XChange*/, 1/*XRate*/, 1/*YChange*/, 1/*YRate*/, SPRITE_DETECT_EDGE | SPRITE_X_KEEPIN | SPRITE_Y_KEEPIN);
							Sprites.AddSprite(&currentsprite[0]);
				}
			}
		}
		else
		{
			if (SpriteOrText == '2')
			{
				SpriteOrText = '1';
				FastLED.clear(true);
				FastLED.show();
			}

			myText.toUpperCase();
			//Serial.println(myText);
			uint8_t msglen = myText.length();
			TxtLen = msglen + LEADSPACE + 1;
			for (uint8_t i = 0; i < msglen; i++)
			{
				TxtMsg[i + LEADSPACE + 1] = myText[i];
			}

			while ((TxtLen + msglen + GAPSPACE) < TotalMessageBytes)
			{
				for (uint8_t i = 0; i < GAPSPACE; i++)
				{
					TxtMsg[TxtLen + i] = ' ';
				}
				TxtLen = TxtLen + GAPSPACE;
				for (uint8_t i = 0; i < msglen; i++)
				{
					TxtMsg[TxtLen + i] = myText[i];
				}
				TxtLen = TxtLen + msglen;
			}
		}

	}
	SendPage(myText);

}


