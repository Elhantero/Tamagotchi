
#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <Math.h>
#include <windows.h>
#include <shellapi.h>
#include <ctime>
#include <vector>
#include "map.h"

using namespace sf;
Clock mytime2;
int energy, deep, weight, respect, scalp, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13;
int flag;
String dlgtext, dlgtext2;
time_t t;
int questResult;

int v(int& year, int& mes, int& day, int& ch)
{

	int mm[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (!(year % 4))
	{
		mm[1] = 29;
		year = year * 366;
	}
	else year = year * 365;

	bool chflag = true;

	if (day>mm[mes - 1]) {
		std::cout << "Дата неверна!" << std::endl;
		chflag = false;
	}
	if (mes <= 0 || mes>12) {
		std::cout << "Месяц неверен!" << std::endl;
		chflag = false;
	}

	if (year<0) {
		std::cout << "Год неверен!" << std::endl;
		chflag = false;
	}


	if (chflag) {
		int i = 0;
		int sm = 0;
		while (i < mes - 1)
		{
			sm = sm + mm[i];
			i++;
		}
		int res = ((year + sm + day) * 24) + ch;
		return res;
	}
	return -1;
}

int load()
{
	std::ifstream f("num.txt");
	f >> energy;
	f >> deep;
	f >> weight;
	f >> respect;
	f >> scalp;
	return 0;
}

int save()
{
	remove("num.txt");
	std::ofstream f;
	f.open("num.txt");
	f << energy << " " << deep << " " << weight << " " << respect << " " << scalp << " " << std::endl;
	f.close();
	return 0;
}

int random(int min, int max) //range : [min, max)
{
	return min + rand() % (max - min);
}

String toString(Int32 integer)
{
	char numstr[10]; // enough to hold all numbers up to 32-bits
	sprintf_s(numstr, "%i", integer);
	return numstr;
}

int save_t()
{
	time(&t);
	//v(int& year, int& mes, int& day, int& ch)
	int month = localtime(&t)->tm_mon;
	int hour = localtime(&t)->tm_hour;
	int day = localtime(&t)->tm_mday;
	int ye = localtime(&t)->tm_year;

	int sum = v(ye, month, day, hour);
	remove("date.txt");
	std::ofstream f;
	f.open("date.txt");
	f << sum << std::endl;
	f.close();
	return 0;
}

int update_me()
{
	time(&t);
	int sum1, sum2, sum3;
	std::ifstream f("date.txt");
	f >> sum1;
	int month1 = localtime(&t)->tm_mon;
	int hour1 = localtime(&t)->tm_hour;
	int day1 = localtime(&t)->tm_mday;
	int ye1 = localtime(&t)->tm_year;

	sum2 = v(ye1, month1, day1, hour1);
	sum3 = sum2 - sum1; // сколько прошло часов
	std::cout << " " << sum3;
	if (sum3 > 0) // если прошло больше 0 часов
	{

		deep = deep - sum3 * 5;
		weight = weight - sum3 * 10;
		energy = 100;
		dlgtext = L"Вы долго не заходили в игру\nПоказатели изменились!\nВес: -" + toString(sum3 * 10) + L"\nПещера: -" + toString(sum3 * 5) + L"\nЭнергия восполнилась до 100";
	}
	save_t();
	save();
	return 0;
}

void menu(RenderWindow & window) {

	Texture menuBackground;
	Texture buttonStartGame;
	Texture buttonContinueGame;
	Texture orcRun;

	menuBackground.loadFromFile("images/menuBack.jpg");
	buttonStartGame.loadFromFile("images/buttonNewGame.png");
	buttonContinueGame.loadFromFile("images/buttonContinueGame.png");
	orcRun.loadFromFile("images/orcRun.png");

	Sprite  menu1(buttonStartGame);
	Sprite	menu2(buttonContinueGame);
	Sprite	menuBg(menuBackground);
	Sprite  orcRunSprite(orcRun);


	SoundBuffer surpriseBuffer;//создаём буфер для звука
	surpriseBuffer.loadFromFile("sound/SURPRISE_MOTHERFUCKER.ogg");//загружаем в него звук
	Sound surpriseSound(surpriseBuffer);//создаем звук и загружаем в него звук из буфера

	SoundBuffer clickBuffer;//создаём буфер для звука
	clickBuffer.loadFromFile("sound/click.ogg");//загружаем в него звук
	Sound clickSound(clickBuffer);//создаем звук и загружаем в него звук из буфера



	Clock mytime;

	float currentFrame = 0;

	bool isMenu = 1;
	int menuNum = 0;

	menu1.setTextureRect(IntRect(0, 0, 392, 100));
	menu1.setPosition(450, 500);
	menu2.setTextureRect(IntRect(0, 0, 392, 100));
	menu2.setPosition(450, 620);
	menuBg.setPosition(0, 0);
	orcRunSprite.setTextureRect(IntRect(0, 0, 437, 310));//437(*7) *310 px
	orcRunSprite.setPosition(20, 480);

	//remove("images/num.txt");
	//////////////////////////////МЕНЮ///////////////////



	while (isMenu)
	{
		//X13 = GetPrivateProfileInt("Section", "Key", 13, "num.ini");

		float time = mytime.getElapsedTime().asMicroseconds();
		mytime.restart();
		time = time / 800;

		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
		//std::cout << pixelPos.x << "\n";//смотрим на координату Х позиции курсора в консоли (она не будет больше ширины окна)
		//std::cout << pos.x << "\n";
		//std::cout << pos.x << " | " << pos.y << "\n";

		menuNum = 0;
		window.clear();

		if (orcRunSprite.getGlobalBounds().contains(pos.x, pos.y))//и при этом координата курсора попадает в спрайт
		{
			currentFrame += 0.01*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
			if (currentFrame > 7) currentFrame -= 7; //если пришли к третьему кадру - откидываемся назад.
			orcRunSprite.setTextureRect(IntRect(437 * int(currentFrame), 0, 437, 310));
		}




		if (menu1.getGlobalBounds().contains(pos.x, pos.y))//и при этом координата курсора попадает в спрайт
		{
			menu1.setTextureRect(IntRect(392, 0, 784, 100));
			menuNum = 1;
		}
		else {
			menu1.setTextureRect(IntRect(0, 0, 392, 100));
		}

		if (menu2.getGlobalBounds().contains(pos.x, pos.y))//и при этом координата курсора попадает в спрайт
		{
			menu2.setTextureRect(IntRect(392, 0, 784, 100));
			menuNum = 2;
		}
		else {
			menu2.setTextureRect(IntRect(0, 0, 392, 100));
		}


		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) {
				isMenu = false; energy = 100; weight = random(35, 45); deep = random(27, 33); scalp = 0; respect = random(20, 30);
				save();
			}//NEW GAME

			if (menuNum == 2) {
				isMenu = false; load(); flag = 229;  //window.draw(menuBg); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); 
			} // CONTINUE
			if (orcRunSprite.getGlobalBounds().contains(pos.x, pos.y)) {
				surpriseSound.play();
			}
		}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menu1.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play();}
			if (menu2.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(orcRunSprite);
		window.display();
	}
}

int finalBattle(RenderWindow & window, int h) {

	
	srand(time(0));

	Texture arenaBackTexture;
	Texture arenaHeadsTexture;
	Texture heroBarTexture;
	Texture enemyBarTexture;

	Texture buttonAttackTexture;
	Texture buttonDefTexture;
	Texture buttonHFightTexture;

	Texture shamanTexture;
	Texture shamanDeadTexture;
	Texture shamanWinexture;

	Texture orcArenaTexture;
	Texture orcWinTexture;
	Texture orcDeadTexture;

	Texture looserBackTexture;
	Texture winnreBackTexture;

	arenaBackTexture.loadFromFile("images/arenaBack.jpg");
	arenaHeadsTexture.loadFromFile("images/arenaHeads.png");
	heroBarTexture.loadFromFile("images/heroBar.png");// 323*76
	enemyBarTexture.loadFromFile("images/enemyBar.png");// 323*76
	buttonAttackTexture.loadFromFile("images/buttonAttack.png");	 //82(*2) *83
	buttonDefTexture.loadFromFile("images/buttonDef.png");			//82(*2) *83
	buttonHFightTexture.loadFromFile("images/buttonFight.png");		//82(*2) *83
	shamanTexture.loadFromFile("images/shaman.png");		//532(*10) *350
	shamanDeadTexture.loadFromFile("images/shamanDead.png");		//430(*10) *350
	shamanWinexture.loadFromFile("images/shamanWin.png");//514(*11) *400
	orcArenaTexture.loadFromFile("images/orcArena.png");//307(*7) *200
	orcWinTexture.loadFromFile("images/orcWin.png");//306(*7) *320
	orcDeadTexture.loadFromFile("images/orcDead.png");//414(*7) *200
	looserBackTexture.loadFromFile("images/looser.jpg");
	winnreBackTexture.loadFromFile("images/victory.jpg");

	Sprite arenaBackSprite(arenaBackTexture);
	Sprite arenaHeadsSprite(arenaHeadsTexture);
	Sprite heroBarSprite(heroBarTexture);
	Sprite enemyBarSprite(enemyBarTexture);
	Sprite buttonAttackkSprite1, buttonAttackkSprite2, buttonAttackkSprite3;
	Sprite buttonDefSprite1, buttonDefSprite2, buttonDefSprite3;
	Sprite buttonHFightSprite;
	Sprite shamanSprite(shamanTexture);
	Sprite shamanDeadSprite(shamanDeadTexture);
	Sprite shamanWinSprite(shamanWinexture);
	Sprite orcArenaSprite(orcArenaTexture);
	Sprite orcWinSprite(orcWinTexture);
	Sprite orcDeadSprite(orcDeadTexture);
	Sprite looserBackSprite(looserBackTexture);
	Sprite winnerBackSprite(winnreBackTexture);

	SoundBuffer clickBuffer;//создаём буфер для звука
	clickBuffer.loadFromFile("sound/click.ogg");//загружаем в него звук
	Sound clickSound(clickBuffer);//создаем звук и загружаем в него звук из буфера

	Music musicMortal;//создаем объект музыки
	musicMortal.openFromFile("sound/mortal.ogg");
	musicMortal.play();//воспроизводим музыку

	Music musicChamp;//создаем объект музыки
	musicChamp.openFromFile("sound/champ.ogg");
	

	bool attack = false, def = false;

	float heroHealth = h;
	float enemyHealth = 1500;

	float heroPower = heroHealth / 15;
	float enemyPower = enemyHealth / 25;

	float heroHit = 0;
	float heroDef = 0;

	float enemyHit = 0;
	float enemyDef = 0;

	Clock mytime;
	double currentFrame = 0;
	double currentFrame2 = 0;
	
	Font font;//шрифт 
	font.loadFromFile("font/Ycfhzibd.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 30);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setFillColor(Color::White);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
								  //text.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	text.setString("");//задает строку тексту
	text.setPosition(170, 55);//задаем позицию текста

	Text text2("", font, 30);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text2.setFillColor(Color::White);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text2.setString("");//задает строку тексту
	text2.setPosition(1050, 55);//задаем позицию текста


	std::ostringstream playerScoreString;    // объявили переменную
	playerScoreString << heroHealth;		//занесли в нее число очков, то есть формируем строку
	text.setString("" + playerScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 

	std::ostringstream playerScoreString2;    // объявили переменную
	playerScoreString2 << enemyHealth;		//занесли в нее число очков, то есть формируем строку
	text2.setString("" + playerScoreString2.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 

	////////////////// Health Bars

	heroBarSprite.setTextureRect(IntRect(0, 0, 323 * 1, 76));
	heroBarSprite.setPosition(133, 39);

	enemyBarSprite.setTextureRect(IntRect(0 + 0, 0, 323, 76));
	enemyBarSprite.setPosition(825 + 0, 39);


	//////////////////////// Button Attack

	buttonAttackkSprite1.setTexture(buttonAttackTexture);
	buttonAttackkSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonAttackkSprite1.setPosition(750, 320);

	buttonAttackkSprite2.setTexture(buttonAttackTexture);
	buttonAttackkSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonAttackkSprite2.setPosition(750, 420);

	buttonAttackkSprite3.setTexture(buttonAttackTexture);
	buttonAttackkSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonAttackkSprite3.setPosition(750, 520);

	//////////////////// Button Def

	buttonDefSprite1.setTexture(buttonDefTexture);
	buttonDefSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonDefSprite1.setPosition(450, 320);

	buttonDefSprite2.setTexture(buttonDefTexture);
	buttonDefSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonDefSprite2.setPosition(450, 420);

	buttonDefSprite3.setTexture(buttonDefTexture);
	buttonDefSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonDefSprite3.setPosition(450, 520);

	////////////////////////  Button Fight

	buttonHFightSprite.setTexture(buttonHFightTexture);
	buttonHFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonHFightSprite.setPosition(600, 420);


	shamanSprite.setTextureRect(IntRect(0 * 532, 0, 532, 350));
	shamanSprite.setPosition(650, 300);

	shamanDeadSprite.setTextureRect(IntRect(0 * 430, 0, 430, 350));
	shamanDeadSprite.setPosition(1725, 350);

	shamanWinSprite.setTextureRect(IntRect(0 * 514, 0, 514, 400));
	shamanWinSprite.setPosition(1725, 350);

	orcWinSprite.setPosition(2100, 420);
	orcDeadSprite.setPosition(2100, 420);

	looserBackSprite.setPosition(2100, 420);
	winnerBackSprite.setPosition(2100, 420);


	while (window.isOpen())
	{
		float time = mytime.getElapsedTime().asMicroseconds();
		mytime.restart();
		time = time / 800;

	

		if (enemyHealth > 0 && heroHealth > 0) {

			currentFrame += 0.008*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
			if (currentFrame > 10) currentFrame -= 10; //если пришли к 10 кадру - откидываемся назад.
			shamanSprite.setTextureRect(IntRect(532 * int(currentFrame), 0, 532, 350));

			orcArenaSprite.setPosition(100, 420);
			currentFrame2 += 0.008*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
			if (currentFrame2 > 7) currentFrame2 -= 7; //если пришли к 10 кадру - откидываемся назад.
			orcArenaSprite.setTextureRect(IntRect(307 * int(currentFrame2), 0, 307, 200));
		}
		else if(enemyHealth<=0 && heroHealth > 0)
		{
			shamanSprite.setPosition(1650, 1300);
			shamanDeadSprite.setPosition(725, 350);
			currentFrame += 0.008*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
			if (currentFrame > 10) currentFrame = 9;
			shamanDeadSprite.setTextureRect(IntRect(430 * int(currentFrame), 0, 430, 350));

			orcArenaSprite.setPosition(2100, 420);
			orcWinSprite.setPosition(120, 320);
			currentFrame2 += 0.008*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
			if (currentFrame2 > 7) currentFrame2 = 6;
			orcWinSprite.setTextureRect(IntRect(306 * int(currentFrame2), 0, 306, 320));

			winnerBackSprite.setPosition(0, 0);
		}

		else if(enemyHealth > 0 && heroHealth <= 0)
		{
			musicMortal.stop();

			shamanSprite.setPosition(1650, 1300);
			shamanWinSprite.setPosition(725, 250);
			currentFrame += 0.01*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
			if (currentFrame > 11) currentFrame = 10; //если пришли к 10 кадру - откидываемся назад.
			shamanWinSprite.setTextureRect(IntRect(514 * int(currentFrame), 0, 514, 400));


			orcArenaSprite.setPosition(1650, 1300);
			orcDeadSprite.setPosition(100, 450);
			currentFrame2 += 0.01*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
			if (currentFrame2 > 7) currentFrame2 = 6; //если пришли к 10 кадру - откидываемся назад.
			orcDeadSprite.setTextureRect(IntRect(414 * int(currentFrame2), 0, 414, 200));

			looserBackSprite.setPosition(0, 0);
		}



		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
		//std::cout << pixelPos.x << "\n";//смотрим на координату Х позиции курсора в консоли (она не будет больше ширины окна)
		//std::cout << pos.x << "\n";

		sf::Event startPage;
		while (window.pollEvent(startPage))
		{
			if (startPage.type == sf::Event::Closed)
				window.close();
			
			if (startPage.type == startPage.MouseButtonReleased && startPage.mouseButton.button == Mouse::Left)
			{
				if (looserBackSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
				{
					return 0;
				}
			}
			if (startPage.type == startPage.MouseButtonReleased && startPage.mouseButton.button == Mouse::Left)
			{
				if (winnerBackSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
				{
					return 0;
				}
			}

			if (startPage.type == startPage.MouseButtonReleased && startPage.mouseButton.button == Mouse::Left)
			{
				if (buttonAttackkSprite1.getGlobalBounds().contains(pos.x, pos.y)) clickSound.play();
				if (buttonAttackkSprite2.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonAttackkSprite3.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonDefSprite1.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonDefSprite2.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonDefSprite3.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonHFightSprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
			}

			

			if (attack == true && def == true) {
				if (startPage.type == startPage.MouseButtonReleased && startPage.mouseButton.button == Mouse::Left) {
					if (buttonHFightSprite.getGlobalBounds().contains(pos.x, pos.y)) {//координата курсора попадает в спрайт
						buttonHFightSprite.setTextureRect(IntRect(1 * 82, 0, 82, 83));

						buttonAttackkSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));
						buttonAttackkSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
						buttonAttackkSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));

						buttonDefSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));
						buttonDefSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
						buttonDefSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));
						
						enemyHit = rand() % 3 + 1;
						enemyDef = rand() % 3 + 1;

						float t1;
						float t2;

						int t3, t4;

						if (heroHit != enemyDef)
						{
							enemyHealth = enemyHealth - heroPower;
							std::ostringstream playerScoreString2;    // объявили переменную
							t3 = enemyHealth;
							playerScoreString2 << t3;		//занесли в нее число очков, то есть формируем строку
							text2.setString("" + playerScoreString2.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
							t1 = enemyHealth / 1500;
							enemyBarSprite.setTextureRect(IntRect(0 + ((1 - t1) * 323), 0, 323, 76));
							enemyBarSprite.setPosition(825 + ((1 - t1) * 323), 39);

						}
						if (heroDef != enemyHit)
						{
							heroHealth = heroHealth - enemyPower;
							std::ostringstream playerScoreString;    // объявили переменную
							t4 = heroHealth;
							playerScoreString << t4;		//занесли в нее число очков, то есть формируем строку
							text.setString("" + playerScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
							t2 = heroHealth / h;

							if (t2 >= -0.1) {
								heroBarSprite.setTextureRect(IntRect(0, 0, 323 * t2, 76));
								heroBarSprite.setPosition(133, 39);
							}

						}
					}
				}
			}

		}


		if (Mouse::isButtonPressed(Mouse::Left))
		{
			/////////////////////////////////////

			if (buttonAttackkSprite1.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonAttackkSprite1.setTextureRect(IntRect(1 * 82, 0, 82, 83)); attack = true; heroHit = 1;
				buttonAttackkSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));
				buttonAttackkSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));

				buttonHFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
			}

			if (buttonAttackkSprite2.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonAttackkSprite2.setTextureRect(IntRect(1 * 82, 0, 82, 83)); attack = true; heroHit = 2;
				buttonAttackkSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
				buttonAttackkSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));

				buttonHFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
			}

			if (buttonAttackkSprite3.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonAttackkSprite3.setTextureRect(IntRect(1 * 82, 0, 82, 83)); attack = true; heroHit = 3;
				buttonAttackkSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
				buttonAttackkSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));

				buttonHFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
			}

			////////////////////////////////

			if (buttonDefSprite1.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonDefSprite1.setTextureRect(IntRect(1 * 82, 0, 82, 83)); def = true; heroDef = 1;
				buttonDefSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));
				buttonDefSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));

				buttonHFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
			}

			if (buttonDefSprite2.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonDefSprite2.setTextureRect(IntRect(1 * 82, 0, 82, 83)); def = true; heroDef = 2;
				buttonDefSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
				buttonDefSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));

				buttonHFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
			}

			if (buttonDefSprite3.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonDefSprite3.setTextureRect(IntRect(1 * 82, 0, 82, 83)); def = true; heroDef = 3;
				buttonDefSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
				buttonDefSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));

				buttonHFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
			}
		}

	


		window.clear();
		window.draw(arenaBackSprite);
		window.draw(heroBarSprite);
		window.draw(enemyBarSprite);
		window.draw(arenaHeadsSprite);

		window.draw(shamanSprite);
		window.draw(shamanDeadSprite);
		window.draw(shamanWinSprite);

		window.draw(orcArenaSprite);
		window.draw(orcWinSprite);
		window.draw(orcDeadSprite);
				
		window.draw(buttonAttackkSprite1);
		window.draw(buttonAttackkSprite2);
		window.draw(buttonAttackkSprite3);
		window.draw(buttonDefSprite1);
		window.draw(buttonDefSprite2);
		window.draw(buttonDefSprite3);
		window.draw(buttonHFightSprite);


		window.draw(text);//рисую этот текст
		window.draw(text2);//рисую этот текст

		window.draw(looserBackSprite);
		window.draw(winnerBackSprite);

		window.display();
	}
	return 0;
}

int questBattle(RenderWindow & window, int heroHP, char difficultyQuest) {

	//difficultyQuest 'e' or 'h',easy and hard
	srand(time(0));

	Texture arenaEasyBackTexture;
	Texture arenaEasyHeadsTexture;

	Texture arenaHardBackTexture;
	Texture arenaHardHeadsTexture;

	Texture heroBarTexture;
	Texture enemyBarTexture;

	Texture buttonAttackTexture;
	Texture buttonDefTexture;
	Texture buttonFightTexture;

	Texture elfEasyTexture;
	Texture elfHardTexture;
	Texture orcArenaTexture;

	arenaEasyBackTexture.loadFromFile("images/questImages/easyElfBack.jpg");
	arenaEasyHeadsTexture.loadFromFile("images/questImages/arenaEasyElfHeads.png");

	arenaHardBackTexture.loadFromFile("images/questImages/hardElfBack.jpg");
	arenaHardHeadsTexture.loadFromFile("images/questImages/arenaHardElfHeads.png");

	heroBarTexture.loadFromFile("images/questImages/heroBar.png");// 323*76
	enemyBarTexture.loadFromFile("images/questImages/enemyBar.png");// 323*76
	buttonAttackTexture.loadFromFile("images/questImages/buttonAttack.png");	 //82(*2) *83
	buttonDefTexture.loadFromFile("images/questImages/buttonDef.png");			//82(*2) *83
	buttonFightTexture.loadFromFile("images/questImages/buttonFight.png");		//82(*2) *83

	elfEasyTexture.loadFromFile("images/questImages/elfEasy.png");		//396(*5) *200
	elfHardTexture.loadFromFile("images/questImages/elfHard.png");		//343(*5) *200
	orcArenaTexture.loadFromFile("images/questImages/orcArena.png");//307(*7) *200

	Sprite arenaEasyBackSprite(arenaEasyBackTexture);
	Sprite arenaEasyHeadsSprite(arenaEasyHeadsTexture);

	Sprite arenaHardBackSprite(arenaHardBackTexture);
	Sprite arenaHardHeadsSprite(arenaHardHeadsTexture);

	Sprite heroBarSprite(heroBarTexture);
	Sprite enemyBarSprite(enemyBarTexture);
	Sprite buttonAttackkSprite1, buttonAttackkSprite2, buttonAttackkSprite3;
	Sprite buttonDefSprite1, buttonDefSprite2, buttonDefSprite3;
	Sprite buttonFightSprite;

	Sprite elfEasySprite(elfEasyTexture);
	Sprite elfHardSprite(elfHardTexture);
	Sprite orcArenaSprite(orcArenaTexture);

	SoundBuffer clickBuffer;//создаём буфер для звука
	clickBuffer.loadFromFile("sound/click.ogg");//загружаем в него звук
	Sound clickSound(clickBuffer);//создаем звук и загружаем в него звук из буфера


	bool attack = false, def = false;

	float heroHealth = heroHP;
	float enemyHealth;

	if (difficultyQuest == 'e')
	{
		enemyHealth = 150;
	}
	else if (difficultyQuest = 'h')
	{
		enemyHealth = 200;
	}

	float heroPower = heroHealth / 15;
	float enemyPower = enemyHealth / 25;

	float heroHit = 0;
	float heroDef = 0;

	float enemyHit = 0;
	float enemyDef = 0;

	Clock mytime;
	double currentFrame = 0;
	double currentFrame2 = 0;

	Font font;//шрифт 
	font.loadFromFile("font/Ycfhzibd.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 30);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setFillColor(Color::White);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
									//text.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	text.setString("");//задает строку тексту
	text.setPosition(170, 55);//задаем позицию текста

	Text text2("", font, 30);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text2.setFillColor(Color::White);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text2.setString("");//задает строку тексту
	text2.setPosition(1050, 55);//задаем позицию текста


	std::ostringstream playerScoreString;    // объявили переменную
	playerScoreString << heroHealth;		//занесли в нее число очков, то есть формируем строку
	text.setString("" + playerScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 

	std::ostringstream playerScoreString2;    // объявили переменную
	playerScoreString2 << enemyHealth;		//занесли в нее число очков, то есть формируем строку
	text2.setString("" + playerScoreString2.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 

												   ////////////////// Health Bars
	heroBarSprite.setTextureRect(IntRect(0, 0, 323 * 1, 76));
	heroBarSprite.setPosition(133, 39);

	enemyBarSprite.setTextureRect(IntRect(0 + 0, 0, 323, 76));
	enemyBarSprite.setPosition(825 + 0, 39);

	//////////////////////// Button Attack

	buttonAttackkSprite1.setTexture(buttonAttackTexture);
	buttonAttackkSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonAttackkSprite1.setPosition(750, 320);

	buttonAttackkSprite2.setTexture(buttonAttackTexture);
	buttonAttackkSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonAttackkSprite2.setPosition(750, 420);

	buttonAttackkSprite3.setTexture(buttonAttackTexture);
	buttonAttackkSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonAttackkSprite3.setPosition(750, 520);

	//////////////////// Button Def

	buttonDefSprite1.setTexture(buttonDefTexture);
	buttonDefSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonDefSprite1.setPosition(450, 320);

	buttonDefSprite2.setTexture(buttonDefTexture);
	buttonDefSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonDefSprite2.setPosition(450, 420);

	buttonDefSprite3.setTexture(buttonDefTexture);
	buttonDefSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonDefSprite3.setPosition(450, 520);

	////////////////////////  Button Fight

	buttonFightSprite.setTexture(buttonFightTexture);
	buttonFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
	buttonFightSprite.setPosition(600, 420);

	elfEasySprite.setTextureRect(IntRect(0 * 396, 0, 396, 200));
	elfEasySprite.setPosition(700, 420);

	elfHardSprite.setTextureRect(IntRect(0 * 343, 0, 343, 200));
	elfHardSprite.setPosition(750, 420);


	while (window.isOpen())
	{
		float time = mytime.getElapsedTime().asMicroseconds();
		mytime.restart();
		time = time / 800;

		if (enemyHealth > 0 && heroHealth > 0) {

			if (difficultyQuest == 'e')
			{
				currentFrame += 0.008*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
				if (currentFrame > 5) currentFrame -= 5; //если пришли к 10 кадру - откидываемся назад.
				elfEasySprite.setTextureRect(IntRect(396 * int(currentFrame), 0, 396, 200));
			}
			else if (difficultyQuest = 'h')
			{
				currentFrame += 0.008*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
				if (currentFrame > 5) currentFrame -= 5; //если пришли к 10 кадру - откидываемся назад.
				elfHardSprite.setTextureRect(IntRect(343 * int(currentFrame), 0, 343, 200));
			}


			orcArenaSprite.setPosition(100, 420);
			currentFrame2 += 0.008*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
			if (currentFrame2 > 7) currentFrame2 -= 7; //если пришли к 10 кадру - откидываемся назад.
			orcArenaSprite.setTextureRect(IntRect(307 * int(currentFrame2), 0, 307, 200));
		}
		else if (enemyHealth <= 0 && heroHealth > 0)
		{
			return heroHealth;
		}
		else if (enemyHealth > 0 && heroHealth <= 0)
		{
			return heroHealth;
		}


		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
														 //std::cout << pixelPos.x << "\n";//смотрим на координату Х позиции курсора в консоли (она не будет больше ширины окна)
														 //std::cout << pos.x << "\n";

		sf::Event startPage;
		while (window.pollEvent(startPage))
		{
			if (startPage.type == sf::Event::Closed)
				window.close();

			if (startPage.type == startPage.MouseButtonReleased && startPage.mouseButton.button == Mouse::Left)
			{
				if (buttonAttackkSprite1.getGlobalBounds().contains(pos.x, pos.y)) clickSound.play();
				if (buttonAttackkSprite2.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonAttackkSprite3.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonDefSprite1.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonDefSprite2.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonDefSprite3.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonFightSprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
			}

			if (attack == true && def == true) {
				if (startPage.type == startPage.MouseButtonReleased && startPage.mouseButton.button == Mouse::Left) {
					if (buttonFightSprite.getGlobalBounds().contains(pos.x, pos.y)) {//координата курсора попадает в спрайт
						buttonFightSprite.setTextureRect(IntRect(1 * 82, 0, 82, 83));

						buttonAttackkSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));
						buttonAttackkSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
						buttonAttackkSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));

						buttonDefSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));
						buttonDefSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
						buttonDefSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));

						enemyHit = rand() % 3 + 1;
						enemyDef = rand() % 3 + 1;

						float t1;
						float t2;

						int t3, t4;

						if (heroHit != enemyDef)
						{
							enemyHealth = enemyHealth - heroPower;
							std::ostringstream playerScoreString2;    // объявили переменную
							t3 = enemyHealth;
							playerScoreString2 << t3;		//занесли в нее число очков, то есть формируем строку
							text2.setString("" + playerScoreString2.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
							if (difficultyQuest == 'e') {
								t1 = enemyHealth / 150;
							}
							else if (difficultyQuest == 'h') {
								t1 = enemyHealth / 200;
							}
							enemyBarSprite.setTextureRect(IntRect(0 + ((1 - t1) * 323), 0, 323, 76));
							enemyBarSprite.setPosition(825 + ((1 - t1) * 323), 39);

						}
						if (heroDef != enemyHit)
						{
							heroHealth = heroHealth - enemyPower;
							std::ostringstream playerScoreString;    // объявили переменную
							t4 = heroHealth;
							playerScoreString << t4;		//занесли в нее число очков, то есть формируем строку
							text.setString("" + playerScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
							t2 = heroHealth / heroHP;

							if (t2 >= -0.1) {
								heroBarSprite.setTextureRect(IntRect(0, 0, 323 * t2, 76));
								heroBarSprite.setPosition(133, 39);
							}
						}
					}
				}
			}

		}


		if (Mouse::isButtonPressed(Mouse::Left))
		{
			/////////////////////////////
			if (buttonAttackkSprite1.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonAttackkSprite1.setTextureRect(IntRect(1 * 82, 0, 82, 83)); attack = true; heroHit = 1;
				buttonAttackkSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));
				buttonAttackkSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));

				buttonFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
			}

			if (buttonAttackkSprite2.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonAttackkSprite2.setTextureRect(IntRect(1 * 82, 0, 82, 83)); attack = true; heroHit = 2;
				buttonAttackkSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
				buttonAttackkSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));

				buttonFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
			}

			if (buttonAttackkSprite3.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonAttackkSprite3.setTextureRect(IntRect(1 * 82, 0, 82, 83)); attack = true; heroHit = 3;
				buttonAttackkSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
				buttonAttackkSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));

				buttonFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
			}

			////////////////////////////////

			if (buttonDefSprite1.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonDefSprite1.setTextureRect(IntRect(1 * 82, 0, 82, 83)); def = true; heroDef = 1;
				buttonDefSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));
				buttonDefSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));

				buttonFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
			}

			if (buttonDefSprite2.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonDefSprite2.setTextureRect(IntRect(1 * 82, 0, 82, 83)); def = true; heroDef = 2;
				buttonDefSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
				buttonDefSprite3.setTextureRect(IntRect(0 * 82, 0, 82, 83));

				buttonFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
			}

			if (buttonDefSprite3.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonDefSprite3.setTextureRect(IntRect(1 * 82, 0, 82, 83)); def = true; heroDef = 3;
				buttonDefSprite1.setTextureRect(IntRect(0 * 82, 0, 82, 83));
				buttonDefSprite2.setTextureRect(IntRect(0 * 82, 0, 82, 83));

				buttonFightSprite.setTextureRect(IntRect(0 * 82, 0, 82, 83));
			}
		}

		window.clear();

		if (difficultyQuest == 'e')
		{
			window.draw(arenaEasyBackSprite);
			window.draw(heroBarSprite);
			window.draw(enemyBarSprite);
			window.draw(arenaEasyHeadsSprite);
			window.draw(elfEasySprite);
		}
		else if (difficultyQuest = 'h')
		{
			window.draw(arenaHardBackSprite);
			window.draw(heroBarSprite);
			window.draw(enemyBarSprite);
			window.draw(arenaHardHeadsSprite);
			window.draw(elfHardSprite);
		}

		window.draw(orcArenaSprite);

		window.draw(buttonAttackkSprite1);
		window.draw(buttonAttackkSprite2);
		window.draw(buttonAttackkSprite3);
		window.draw(buttonDefSprite1);
		window.draw(buttonDefSprite2);
		window.draw(buttonDefSprite3);
		window.draw(buttonFightSprite);

		window.draw(text);//рисую этот текст
		window.draw(text2);//рисую этот текст

		window.display();
	}
	return heroHealth;
}

void objectMap(int object, char letter)
{
	while (object>0)
	{
		int randomElementX = rand() % 12;
		int	randomElementY = rand() % 12;
		if (TileMap[randomElementY][randomElementX] == ' ')
		{
			object--;
			TileMap[randomElementY][randomElementX] = letter;
		}
	}
}

int questModul(char difficulty)
{
	//difficulty 'e' or 'h',easy and hard

	sf::RenderWindow window(sf::VideoMode(1280, 800), "Battle Field", sf::Style::Close | sf::Style::Titlebar);
	srand(time(NULL));

	int countPurse;//количество кошельков
	int countChest;//...сундуков
	int countTrap;//...ловушек
	int countFountain;//...фонтанов
	int countEnemy;//...врагов
	int countMeat;///...еда


	if (difficulty == 'e') {
		countPurse = 2;//количество кошельков
		countChest = 1;//...сундуков
		countTrap = 2;//...ловушек
		countFountain = 1;//...фонтанов
		countEnemy = 3;//...врагов
		countMeat = 1;///...еда
	}
	if (difficulty == 'h') {
		countPurse = 3;//количество кошельков
		countChest = 2;//...сундуков
		countTrap = 3;//...ловушек
		countFountain = 1;//...фонтанов
		countEnemy = 3;//...врагов
		countMeat = 1;///...еда
	}

	int HP = 300;
	int PW = 30;
	int Gold = 0;

	sf::Image mapImage;
	mapImage.loadFromFile("images/questImages/mapTile.jpg");
	sf::Texture map;
	map.loadFromImage(mapImage);
	sf::Sprite mapSprite;
	mapSprite.setTexture(map);

	sf::Image mapImageTop;
	mapImageTop.loadFromFile("images/questImages/mapTop.png");
	sf::Texture mapTop;
	mapTop.loadFromImage(mapImageTop);
	sf::Sprite mapSpriteTop;
	mapSpriteTop.setTexture(mapTop);

	sf::Texture heroTexture;
	heroTexture.loadFromFile("images/questImages/hero.png");
	sf::Sprite heroSprite;
	heroSprite.setTexture(heroTexture);
	heroSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	int px, py;
	px = 1 + rand() % 7;
	py = 1 + rand() % 7;
	heroSprite.setPosition(0 + 15 + 64 * px, 0 + 15 + 64 * py);

	for (int i = 1; i < HEIGHT_MAP_TOP - 1; i++)
	{
		for (int j = 1; j < WiDTH_MAP_TOP - 1; j++)
		{
			TileMap[i][j] = ' ';
		}
	}

	for (int i = 1; i < HEIGHT_MAP_TOP-1; i++)
	{
		for (int j = 1; j < WiDTH_MAP_TOP-1; j++)
		{
			TileMapTOP[i][j] = ' ';
		}
	}

	sf::Texture backTexture;
	backTexture.loadFromFile("images/questImages/backBF.png");
	sf::Sprite backSprite(backTexture);

	sf::Texture buttonLooserTexture;
	buttonLooserTexture.loadFromFile("images/questImages/buttonLooser.png");
	sf::Sprite buttonLooserSprite(buttonLooserTexture);
	buttonLooserSprite.setPosition(1500, 0);

	sf::Texture buttonWinnerTexture;
	buttonWinnerTexture.loadFromFile("images/questImages/buttonWinner.png");
	sf::Sprite buttonWinnerSprite(buttonWinnerTexture);
	buttonWinnerSprite.setPosition(1500, 0);


	objectMap(countPurse, 'P');
	objectMap(countChest, 'C');
	objectMap(countTrap, 'T');
	objectMap(countFountain, 'F');
	objectMap(countEnemy, 'E');
	objectMap(countMeat, 'M');

	sf::Font font;
	font.loadFromFile("font/Ycfhzibd.ttf");

	sf::Text textGold("", font, 40);
	textGold.setFillColor(sf::Color::Black);
	textGold.setPosition(1100, 320);
	std::ostringstream playerScoreGold;
	playerScoreGold << Gold;
	textGold.setString("" + playerScoreGold.str());

	sf::Text textHP("", font, 40);
	textHP.setFillColor(sf::Color::Black);
	textHP.setPosition(1100, 410);
	std::ostringstream playerScoreHP;
	playerScoreHP << HP;
	textHP.setString("" + playerScoreHP.str());

	sf::Text textLog("", font, 18);
	textLog.setFillColor(sf::Color::Black);
	textLog.setPosition(850, 215);
	textLog.setString("QUEST LOG");

	int randomPurse = 0;
	int randomChest = 0;
	bool meal = false;

	

	while (window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			if (evnt.type == evnt.Closed)
			{
				window.close();
			}
		}

		window.clear();

		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);

		for (int i = 0; i < HEIGHT_MAP; i++)
		{
			for (int j = 0; j < WiDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ') mapSprite.setTextureRect(sf::IntRect(64 * 0, 0, 64, 64));
				if (TileMap[i][j] == '0') mapSprite.setTextureRect(sf::IntRect(64 * 1, 0, 64, 64));
				if (TileMap[i][j] == 'P' || TileMap[i][j] == 'p') mapSprite.setTextureRect(sf::IntRect(64 * 2, 0, 64, 64));
				if (TileMap[i][j] == 'C' || TileMap[i][j] == 'c') mapSprite.setTextureRect(sf::IntRect(64 * 3, 0, 64, 64));
				if (TileMap[i][j] == 'T' || TileMap[i][j] == 't') mapSprite.setTextureRect(sf::IntRect(64 * 4, 0, 64, 64));
				if (TileMap[i][j] == 'F' || TileMap[i][j] == 'f') mapSprite.setTextureRect(sf::IntRect(64 * 5, 0, 64, 64));
				if (difficulty == 'e') {
					if (TileMap[i][j] == 'E' || TileMap[i][j] == 'e') mapSprite.setTextureRect(sf::IntRect(64 * 6, 0, 64, 64));
				}
				else if (difficulty == 'h')
				{
					if (TileMap[i][j] == 'E' || TileMap[i][j] == 'e') mapSprite.setTextureRect(sf::IntRect(64 * 7, 0, 64, 64));
				}
				if (TileMap[i][j] == 'M' || TileMap[i][j] == 'm') mapSprite.setTextureRect(sf::IntRect(64 * 8, 0, 64, 64));


				mapSprite.setPosition(15 + j * 64, 15 + i * 64);
				window.draw(mapSprite);
			}
		}

		for (int i = 0; i < HEIGHT_MAP_TOP; i++)
		{
			for (int j = 0; j < WiDTH_MAP_TOP; j++)
			{
				if (TileMapTOP[i][j] == ' ') mapSpriteTop.setTextureRect(sf::IntRect(0, 0, 64, 64));
				if (TileMapTOP[i][j] == '0') mapSpriteTop.setTextureRect(sf::IntRect(64, 0, 64, 64));
				mapSpriteTop.setPosition(15 + j * 64, 15 + i * 64);
				window.draw(mapSpriteTop);
			}
		}
	

		int coordX = (heroSprite.getPosition().x - 15) / 64;
		int coordY = (heroSprite.getPosition().y - 15) / 64;
		//std::cout << "x = " << coordX << "\n";
		//std::cout << "y = " << coordY << "\n";
		//std::cout << "gold = " << Gold << "\n";


		if (TileMapTOP[coordY][coordX] == ' ')
		{
			TileMapTOP[coordY][coordX] = '0';
			window.draw(mapSpriteTop);
			std::cout << TileMapTOP[coordX][coordY] << "\n";
		}

		if (Gold >= 0 && meal == false) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && evnt.type == sf::Event::KeyReleased)
			{
				if (TileMap[coordY][coordX - 1] != '0')
				{
					heroSprite.move(-64, 0);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && evnt.type == sf::Event::KeyReleased)
			{
				if (TileMap[coordY][coordX + 1] != '0')
				{
					heroSprite.move(64, 0);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && evnt.type == sf::Event::KeyReleased)
			{
				if (TileMap[coordY - 1][coordX] != '0')
				{
					heroSprite.move(0, -64);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && evnt.type == sf::Event::KeyReleased)
			{
				if (TileMap[coordY + 1][coordX] != '0')
				{
					heroSprite.move(0, 64);
				}
			}
		}

		if (TileMap[coordY][coordX] == 'P' && TileMapTOP[coordY][coordX] == '0')
		{

			TileMap[coordY][coordX] = 'p';
			randomPurse = 1 + rand() % 8;
			Gold = Gold + randomPurse;
			std::ostringstream playerScoreGold;
			playerScoreGold << Gold;
			textGold.setString(playerScoreGold.str());
			textLog.setString("You found a lost purse with " + toString(randomPurse) + " gold");
		}

		if (TileMap[coordY][coordX] == 'C' && TileMapTOP[coordY][coordX] == '0')
		{
			TileMap[coordY][coordX] = 'c';
			randomChest = 10 + rand() % 30;
			Gold = Gold + randomChest;
			std::ostringstream playerScoreGold;
			playerScoreGold << Gold;
			textGold.setString(playerScoreGold.str());
			textLog.setString("You found a lost elfs chest with " + toString(randomChest) + " gold");
		}

		if (TileMap[coordY][coordX] == 'T' && TileMapTOP[coordY][coordX] == '0')
		{
			HP = HP - HP / 4;
			TileMap[coordY][coordX] = 't';
			std::ostringstream playerScoreHP;
			playerScoreHP << HP;
			textHP.setString(playerScoreHP.str());
			textLog.setString("You are trapped and lost 25% HP");
		}

		if (TileMap[coordY][coordX] == 'F' && TileMapTOP[coordY][coordX] == '0')
		{
			HP = 300;
			TileMap[coordY][coordX] = 'f';
			std::ostringstream playerScoreHP;
			playerScoreHP << HP;
			textHP.setString(playerScoreHP.str());
			textLog.setString("You found the fountain of life \n\nand restored all HP");
		}

		if (TileMap[coordY][coordX] == 'E' && TileMapTOP[coordY][coordX] == '0')
		{
			TileMap[coordY][coordX] = 'e';
			if (difficulty == 'e') {
				HP = questBattle(window, HP, 'e');
			}
			else if (difficulty == 'h') {
				HP = questBattle(window, HP, 'h');
			}

			if (HP>0)
			{
				textLog.setString("The elf attacked you, but you were \n\nable to defeat");
			}
			else
			{
				textLog.setString("The elf attacked you, defeated and drove \n\nyou out of the forest");
				Gold = 0 - rand() % 7 - 1;
				std::ostringstream playerScoreGold;
				playerScoreGold << Gold;
				textGold.setString(playerScoreGold.str());
				buttonLooserSprite.setPosition(0, 0);

			}
			std::ostringstream playerScoreHP;
			playerScoreHP << HP;
			textHP.setString(playerScoreHP.str());
		}

		if (TileMap[coordY][coordX] == 'M' && TileMapTOP[coordY][coordX] == '0')
		{
			TileMap[coordY][coordX] = 'm';
			textLog.setString("Victory!!!\n\nYou found the beast and got food");
			meal = true;
			buttonWinnerSprite.setPosition(0, 0);
		}

		if (evnt.type == evnt.MouseButtonReleased && evnt.mouseButton.button == Mouse::Left)
		{
			if (buttonLooserSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				return Gold;
			}
		}

		if (evnt.type == evnt.MouseButtonReleased && evnt.mouseButton.button == Mouse::Left)
		{
			if (buttonWinnerSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				return Gold;
				
			}
		}

		window.draw(backSprite);
		window.draw(textGold);
		window.draw(textHP);
		window.draw(textLog);
		window.draw(heroSprite);
		window.draw(buttonLooserSprite);
		window.draw(buttonWinnerSprite);
		window.display();
	}
}

int main()
{
	RenderWindow window(sf::VideoMode(1280, 800), "TAMAGOTCHI", sf::Style::Close | sf::Style::Titlebar);
	menu(window);

	setlocale(LC_ALL, "rus");

	Font font;
	font.loadFromFile("font/comic.ttf");
	Font fontH;
	fontH.loadFromFile("font/FEASFBRG_0.TTF");
	 
	Text arenaText("Arena",fontH,60);
	arenaText.setFillColor(sf::Color::Color(36, 76, 75, 255));
	Text weightText("Weight", fontH, 60);
	weightText.setFillColor(sf::Color::Color(36, 76, 75, 255));
	Text caveText("Cave", fontH, 60);
	caveText.setFillColor(sf::Color::Color(36, 76, 75, 255));

	weightText.setPosition(2260, 95);
	caveText.setPosition(2270, 95);
	arenaText.setPosition(2260, 95);

	Text text("", font, 20);
	Text text2("", font, 20);
	Text text3("", font, 20);
	Text text4("", font, 20);
	Text text5("", font, 20);
	Text text6("", font, 20);
	Text text7("", font, 20);
	Text text8("GAME OVER", font, 20);
	text.setCharacterSize(20);
	text2.setCharacterSize(20);
	text3.setCharacterSize(20);
	text4.setCharacterSize(20);
	text5.setCharacterSize(20);
	text6.setCharacterSize(18);
	text7.setCharacterSize(18);
	text8.setCharacterSize(40); //GAME OVER
	text.setFillColor(sf::Color::Black);
	text2.setFillColor(sf::Color::Black);
	text3.setFillColor(sf::Color::Black);
	text4.setFillColor(sf::Color::Black);
	text5.setFillColor(sf::Color::Black);
	text6.setFillColor(sf::Color::Black);
	text7.setFillColor(sf::Color::Black);


	///////////Textures of backgrounds for main page of game
	Texture mainBackTexture;
	Texture oldPaperTexture;
	///////////Textures of top 3 buttons
	Texture buttonWeightTexture;
	Texture buttonCaveTexture;
	Texture buttonArenaTexture;
	/////////Texture of background for left block
	Texture backLeftBlockTexture;
	//////// Texture of main hero
	Texture orcTexture;
	//////// Textures of bottom 2 icons
	Texture respectIconTexture;
	Texture energyIconTexture;
	/////////Textures for 6 buttons of change way
	Texture buttonEasyArenaTexture;
	Texture buttonHardArenaTexture;
	Texture buttonMainBattleTexture;

	Texture buttonEasyHuntTexture;
	Texture buttonHardHuntTexture;

	Texture buttonEasyWorkTexture;
	Texture buttonHardWorkTexture;

	Texture buttonManualTexture;
	Texture manualTexture;
	Texture buttonBackTexture;

	Texture notEnoughEnergyTexture;
	

	mainBackTexture.loadFromFile("images/mainGameBack.png");
	oldPaperTexture.loadFromFile("images/oldPaper.jpg");
	notEnoughEnergyTexture.loadFromFile("images/notenergy.png");
	buttonWeightTexture.loadFromFile("images/buttonWeight.png");      //130(*2) * 134 px
	buttonCaveTexture.loadFromFile("images/buttonDeepCave.png");      //130(*2) * 134 px
	buttonArenaTexture.loadFromFile("images/buttonArena.png");      //130(*2) * 134 px
	backLeftBlockTexture.loadFromFile("images/backLeft.jpg");//497(*7) * 565 px
	orcTexture.loadFromFile("images/orc.png");//220(*8) *195;
	respectIconTexture.loadFromFile("images/respect.png"); // 130(*10) *134 px
	energyIconTexture.loadFromFile("images/energy.png");//130(*10) *134 px

	buttonEasyArenaTexture.loadFromFile("images/arenaEasy.png"); //139(*2) *175 px
	buttonHardArenaTexture.loadFromFile("images/arenaHard.png"); //139(*2) *175 px
	buttonMainBattleTexture.loadFromFile("images/buttonMainBattle.png");//229(*2) *186

	buttonEasyHuntTexture.loadFromFile("images/huntEasy.png"); //139(*2) *175 px
	buttonHardHuntTexture.loadFromFile("images/huntHard.png"); //139(*2) *175 px

	buttonEasyWorkTexture.loadFromFile("images/workEasy.png"); //139(*2) *175 px
	buttonHardWorkTexture.loadFromFile("images/workHard.png"); //139(*2) *175 px

	buttonManualTexture.loadFromFile("images/buttonManual.png");//84(*2) *99 px
	manualTexture.loadFromFile("images/manual.jpg");
	buttonBackTexture.loadFromFile("images/buttonBack.png");//84(*2) *99 px


	Sprite mainBackSprite;
	Sprite oldPaperSprite;
	Sprite buttonWeightSprite;
	Sprite buttonCaweightprite;
	Sprite buttonArenaSprite;
	Sprite backLeftBlockSprite;
	Sprite orcSprite;
	Sprite respectIconSprite;
	Sprite energyIconSprite;

	Sprite buttonEasyArenaSprite;
	Sprite buttonHardArenaSprite;
	Sprite buttonMainBattleSprite;

	Sprite buttonEasyHuntSprite;
	Sprite buttonHardHuntSprite;

	Sprite buttonEasyWorkSprite;
	Sprite buttonHardWorkSprite;

	Sprite buttonManualSprite;
	Sprite manualSprite(manualTexture);
	Sprite buttonBackSprite;

	Sprite notEnoughEnergySprite;
	
	SoundBuffer clickBuffer;//создаём буфер для звука
	clickBuffer.loadFromFile("sound/click.ogg");//загружаем в него звук
	Sound clickSound(clickBuffer);//создаем звук и загружаем в него звук из буфера
	

	notEnoughEnergySprite.setTexture(notEnoughEnergyTexture);
	notEnoughEnergySprite.setPosition(1450, 250);//450, 250

	mainBackSprite.setTexture(mainBackTexture);
	mainBackSprite.setTextureRect(IntRect(0, 0, 1280, 800));
	mainBackSprite.setPosition(0, 0);

	oldPaperSprite.setTexture(oldPaperTexture);
	oldPaperSprite.setTextureRect(IntRect(0, 0, 1280, 800));
	oldPaperSprite.setPosition(0, 0);

	buttonWeightSprite.setTexture(buttonWeightTexture);
	buttonWeightSprite.setTextureRect(IntRect(0, 0, 130, 134));
	buttonWeightSprite.setPosition(730, 30);

	buttonCaweightprite.setTexture(buttonCaveTexture);
	buttonCaweightprite.setTextureRect(IntRect(0, 0, 130, 134));
	buttonCaweightprite.setPosition(900, 30);

	buttonArenaSprite.setTexture(buttonArenaTexture);
	buttonArenaSprite.setTextureRect(IntRect(0, 0, 130, 134));
	buttonArenaSprite.setPosition(1070, 30);

	backLeftBlockSprite.setTexture(backLeftBlockTexture);
	backLeftBlockSprite.setTextureRect(IntRect(0 * 497, 0, 497, 565));
	backLeftBlockSprite.setPosition(80, 170);

	orcSprite.setTexture(orcTexture);
	orcSprite.setTextureRect(IntRect(0 * 220, 0, 220, 195));
	orcSprite.setPosition(125, 425);

	respectIconSprite.setTexture(respectIconTexture);
	respectIconSprite.setTextureRect(IntRect(8 * 130, 0, 130, 134));
	respectIconSprite.setPosition(830, 650);

	energyIconSprite.setTexture(energyIconTexture);
	energyIconSprite.setTextureRect(IntRect(5 * 130, 0, 130, 134));
	energyIconSprite.setPosition(1000, 650);

	buttonEasyArenaSprite.setTexture(buttonEasyArenaTexture);
	buttonEasyArenaSprite.setTextureRect(IntRect(0 * 139, 0, 139, 175));
	buttonEasyArenaSprite.setPosition(1500, 1500);//(820, 475)

	buttonHardArenaSprite.setTexture(buttonHardArenaTexture);
	buttonHardArenaSprite.setTextureRect(IntRect(0 * 139, 0, 139, 175));
	buttonHardArenaSprite.setPosition(1500, 1500);//(990, 475)
		
	buttonMainBattleSprite.setTexture(buttonMainBattleTexture);
	buttonMainBattleSprite.setTextureRect(IntRect(0 * 229, 0, 229, 186));
	buttonMainBattleSprite.setPosition(1500, 1500);

	buttonEasyHuntSprite.setTexture(buttonEasyHuntTexture);
	buttonEasyHuntSprite.setTextureRect(IntRect(0 * 139, 0, 139, 175));
	buttonEasyHuntSprite.setPosition(1500, 1500);//(820, 475)

	buttonHardHuntSprite.setTexture(buttonHardHuntTexture);
	buttonHardHuntSprite.setTextureRect(IntRect(0 * 139, 0, 139, 175));
	buttonHardHuntSprite.setPosition(1500, 1500);//(990, 475)

	buttonEasyWorkSprite.setTexture(buttonEasyWorkTexture);
	buttonEasyWorkSprite.setTextureRect(IntRect(0 * 139, 0, 139, 175));
	buttonEasyWorkSprite.setPosition(1500, 1500);//(820, 475)

	buttonHardWorkSprite.setTexture(buttonHardWorkTexture);
	buttonHardWorkSprite.setTextureRect(IntRect(0 * 139, 0, 139, 175));
	buttonHardWorkSprite.setPosition(1500, 1500);//(990, 475)

	
	buttonManualSprite.setTexture(buttonManualTexture);
	buttonManualSprite.setTextureRect(IntRect(0 * 84, 0, 84, 99));
	buttonManualSprite.setPosition(50, 660);

	manualSprite.setPosition(2000, 2000);
		
	buttonBackSprite.setTexture(buttonBackTexture);
	buttonBackSprite.setTextureRect(IntRect(0 * 84, 0, 84, 99));
	buttonBackSprite.setPosition(2180, 5);

	while (window.isOpen())
	{
		if (respect < 0) respect = 0;
		if (respect > 100) respect = 100;
		if (energy < 0) energy = 0;
		if (energy > 100) energy = 100;
		if (weight < 0) weight = 0;

		int time2 = mytime2.getElapsedTime().asSeconds();
		if (time2>30)
		{
			energy = energy + 1;
			mytime2.restart();
			save_t();
		}

		if (flag == 229)
		{
			std::cout << "flag 299";
			update_me();
			flag = 228;
		}


		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
		std::cout << pixelPos.x << "\n";//смотрим на координату Х позиции курсора в консоли (она не будет больше ширины окна)
		std::cout << pixelPos.y << "\n";//смотрим на координату Х позиции курсора в консоли (она не будет больше ширины окна)
		//std::cout << pos.x << " | " << pos.y << "\n";

		

		sf::Event startPage;
		while (window.pollEvent(startPage))
		{
			if (startPage.type == sf::Event::Closed)
				window.close();

			if (startPage.type == startPage.MouseButtonReleased && startPage.mouseButton.button == Mouse::Left)
			{
				if (buttonWeightSprite.getGlobalBounds().contains(pos.x, pos.y)) clickSound.play();
				if (buttonCaweightprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonArenaSprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonEasyArenaSprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonHardArenaSprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonMainBattleSprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonEasyHuntSprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonHardHuntSprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonEasyWorkSprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonHardWorkSprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonManualSprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
				if (buttonBackSprite.getGlobalBounds().contains(pos.x, pos.y)) { clickSound.play(); }
			}
		}


		if (scalp >= 0) orcSprite.setTextureRect(IntRect(0 * 220, 0, 220, 195));
		if (scalp >= 1) orcSprite.setTextureRect(IntRect(1 * 220, 0, 220, 195));
		if (scalp >= 3) orcSprite.setTextureRect(IntRect(2 * 220, 0, 220, 195));
		if (scalp >= 5) orcSprite.setTextureRect(IntRect(3 * 220, 0, 220, 195));
		if (scalp >= 10) orcSprite.setTextureRect(IntRect(4 * 220, 0, 220, 195));
		if (scalp >= 15) orcSprite.setTextureRect(IntRect(5 * 220, 0, 220, 195));
		if (scalp >= 20) orcSprite.setTextureRect(IntRect(6 * 220, 0, 220, 195));
		if (scalp >= 25) orcSprite.setTextureRect(IntRect(7 * 220, 0, 220, 195));
			
		if (notEnoughEnergySprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
		{
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				notEnoughEnergySprite.setPosition(1450,250);
			}
		}

		if (buttonManualSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
		{
			buttonManualSprite.setTextureRect(IntRect(84, 0, 84, 99));
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				manualSprite.setPosition(0, 0);
				buttonBackSprite.setPosition(1180, 5);

				weightText.setPosition(2260, 95);
				caveText.setPosition(2270, 95);
				arenaText.setPosition(2260, 95);
			}
		}
		else {
			buttonManualSprite.setTextureRect(IntRect(0, 0, 84, 99));

		}


		if (buttonBackSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
		{
			buttonBackSprite.setTextureRect(IntRect(84, 0, 84, 99));
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				manualSprite.setPosition(2000, 0);
				buttonBackSprite.setPosition(2180, 5);
			}
		}
		else
		{
			buttonBackSprite.setTextureRect(IntRect(0, 0, 84, 99));
		}



		if (buttonWeightSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
		{
			buttonWeightSprite.setTextureRect(IntRect(130, 0, 260, 134));
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				backLeftBlockSprite.setTextureRect(IntRect(0 * 497, 0, 497, 565));

				buttonEasyHuntSprite.setPosition(820, 475);//(820, 475)
				buttonHardHuntSprite.setPosition(990, 475);//(990, 475)

				buttonEasyArenaSprite.setPosition(1500, 1500);//(820, 475)
				buttonHardArenaSprite.setPosition(1500, 1500);//(990, 475)

				buttonEasyWorkSprite.setPosition(1500, 1500);//(820, 475)
				buttonHardWorkSprite.setPosition(1500, 1500);//(990, 475)

				weightText.setPosition(260, 95);
				caveText.setPosition(2270, 95);
				arenaText.setPosition(2260, 95);

				buttonMainBattleSprite.setPosition(1860, 450);

				dlgtext = L"Have you decided to have a light meal?\nYou can hunt for easy prey,\nor try a more serious beast.\nWhat do you choose?";
				dlgtext2 = "";
			}

		}
		else {
			buttonWeightSprite.setTextureRect(IntRect(0, 0, 130, 134));
		}



		if (buttonCaweightprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
		{
			buttonCaweightprite.setTextureRect(IntRect(130, 0, 260, 134));
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				backLeftBlockSprite.setTextureRect(IntRect(3 * 497, 0, 497, 565));

				buttonEasyWorkSprite.setPosition(820, 475);//(820, 475)
				buttonHardWorkSprite.setPosition(990, 475);//(990, 475)

				buttonEasyArenaSprite.setPosition(1500, 1500);//(820, 475)
				buttonHardArenaSprite.setPosition(1500, 1500);//(990, 475)

				buttonEasyHuntSprite.setPosition(1500, 1500);//(820, 475)
				buttonHardHuntSprite.setPosition(1500, 1500);//(990, 475)

				weightText.setPosition(2260, 95);
				caveText.setPosition(270, 95);
				arenaText.setPosition(2260, 95);

				buttonMainBattleSprite.setPosition(1860, 450);

				dlgtext = L"You are in the digger mode.\nUse the two buttons below to select the complexity \nof the job.\nThe faster you dig the faster you'll be tired\n";
				dlgtext2 = "";
			}

		}
		else {
			buttonCaweightprite.setTextureRect(IntRect(0, 0, 130, 134));
		}



		if(scalp<25){
			if (buttonArenaSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonArenaSprite.setTextureRect(IntRect(130, 0, 130, 134));
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					backLeftBlockSprite.setTextureRect(IntRect(4 * 497, 0, 497, 565));

					buttonEasyArenaSprite.setPosition(820, 475);//(820, 475)
					buttonHardArenaSprite.setPosition(990, 475);//(990, 475)

					buttonEasyHuntSprite.setPosition(1500, 1500);//(820, 475)
					buttonHardHuntSprite.setPosition(1500, 1500);//(990, 475)

					buttonEasyWorkSprite.setPosition(1500, 1500);//(820, 475)
					buttonHardWorkSprite.setPosition(1500, 1500);//(990, 475)

					weightText.setPosition(2260, 95);
					caveText.setPosition(2270, 95);
					arenaText.setPosition(260, 95);

					dlgtext = L"You chose the arena mode.\nUse the two buttons at the bottom of the screen to \nselect the difficulty level of your opponent.\nThe harder the opponent is, the higher the reward for it!";
					dlgtext2 = "";
				}
			}
			else {
				buttonArenaSprite.setTextureRect(IntRect(0, 0, 130, 134));
			}
		}
		else
		{
			buttonArenaSprite.setTextureRect(IntRect(130*2, 0, 130, 134));
			if (buttonArenaSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
			{
				buttonArenaSprite.setTextureRect(IntRect(130*3, 0, 130, 134));
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					backLeftBlockSprite.setTextureRect(IntRect(7 * 497, 0, 497, 565));

					buttonEasyArenaSprite.setPosition(1500, 475);//(820, 475)
					buttonHardArenaSprite.setPosition(1500, 475);//(990, 475)

					buttonEasyHuntSprite.setPosition(1500, 1500);//(820, 475)
					buttonHardHuntSprite.setPosition(1500, 1500);//(990, 475)

					buttonEasyWorkSprite.setPosition(1500, 1500);//(820, 475)
					buttonHardWorkSprite.setPosition(1500, 1500);//(990, 475)

					weightText.setPosition(2260, 95);
					caveText.setPosition(2270, 95);
					arenaText.setPosition(260, 95);

					buttonMainBattleSprite.setPosition(860, 450);
					
					dlgtext = L"You did it!!!.\nYou have 25 scalps and now tribe's leader\nis waiting for you.\nGood luck good luck in the main battle of your life!";
					dlgtext2 = "";
				}
			}
		}


		if (buttonMainBattleSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
		{
			buttonMainBattleSprite.setTextureRect(IntRect(229 * 1, 0, 229, 186));
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				finalBattle(window,weight);
			}
		}
		else
		{
			buttonMainBattleSprite.setTextureRect(IntRect(229 * 0, 0, 229, 186));
		}


		
		if (buttonEasyArenaSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
		{
			buttonEasyArenaSprite.setTextureRect(IntRect(139, 0, 139, 175));
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (energy >= 10) {
					if (X7 == 0)
					{
						backLeftBlockSprite.setTextureRect(IntRect(5 * 497, 0, 497, 565));
						X4 = random(0, 101); // WIN RANDOM CHANGE

						if (X4 > 16)
						{
							X1 = random(1, 4); // weight
							X2 = random(2, 5); // STAM
							X3 = random(-3, 3); // respect
							X4 = random(0, 101); // SCALP RANDOM CHANGE
							if (X4 < 16)
							{
								X5 = 1;
							}
							else X5 = 0;
							dlgtext2 = L"Victory over a weak opponent!\nRewards and losses:\nWeight: -" + toString(X1) + L"\nEnergy: -" + toString(X2) + L"\nRespect:  " + toString(X3) + L"\nScalp: " + toString(X5);
							weight = weight - X1;
							energy = energy - X2;
							respect = respect + X3;
							scalp = scalp + X5;
							X7 = 1;
							save();
						}
						else
						{
							X1 = random(4, 9); // weight
							X2 = random(5, 9); // STAM
							X3 = random(-10, 0); // respect
							X5 = 0;
							dlgtext2 = L"Defeat against a weak opponent!\nRewards and losses:\nWeight: -" + toString(X1) + L"\nEnergy: -" + toString(X2) + L"\nRespect:  " + toString(X3) + L"\nScalp: " + toString(X5);
							weight = weight - X1;
							energy = energy - X2;
							respect = respect + X3;
							scalp = scalp + X5;
							X7 = 1;
							save();
						}
					}
				}
				else
				{
					notEnoughEnergySprite.setPosition(450, 250);//450, 250
				}
			}
		}
		else {
			buttonEasyArenaSprite.setTextureRect(IntRect(0 * 139, 0, 139, 175));
			X7 = 0;
		}




		if (buttonHardArenaSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
		{
			buttonHardArenaSprite.setTextureRect(IntRect(139, 0, 139, 175));
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (energy >= 10) {
					if (X8 == 0)
					{
						backLeftBlockSprite.setTextureRect(IntRect(6 * 497, 0, 497, 565));
						X4 = random(0, 101); // RANDOM  CHANGE WIN

						if (X4 > 50) {
							X1 = random(3, 9); // weight
							X2 = random(6, 12); // STAMIN
							X3 = random(5, 12); // respect
							X4 = random(0, 101);//RANDOM CHANGE SCALP

							if (X4 < 31)
							{
								X5 = 1;
							}
							else X5 = 0;

							dlgtext2 = L"Victory over a strong opponent!\nRewards and losses:\nWeight: -" + toString(X1) + L"\nEnergy: -" + toString(X2) + L"\nRespect: -" + toString(X3) + L"\nScalp: " + toString(X5);
							weight = weight - X1;
							energy = energy - X2;
							respect = respect + X3;
							scalp = scalp + X5;
							X8 = 1;
							save();
						}
						else
						{
							X1 = random(5, 13); // weight
							X2 = random(8, 15); // STAMIN
							X3 = random(1, 5); // respect

							dlgtext2 = L"Defeat against a strong opponent!\nRewards and losses:\nWeight: -" + toString(X1) + L"\nEnergy: -" + toString(X2) + L"\nRespect: -" + toString(X3) + L"\nScalp: " + toString(X5);
							weight = weight - X1;
							energy = energy - X2;
							respect = respect + X3;
							scalp = scalp + X5;
							X8 = 1;
							save();
						}
					}
				}
				else
				{
					notEnoughEnergySprite.setPosition(450, 250);//450, 250
				}
			}
		}
		else {
			buttonHardArenaSprite.setTextureRect(IntRect(0 * 139, 0, 139, 175));
			X8 = 0;
		}




		if (buttonEasyHuntSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
		{
			buttonEasyHuntSprite.setTextureRect(IntRect(139, 0, 139, 175));
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (energy >= 10) {
					questResult = questModul('e');
					if (questResult >= 0) {
						if (X9 == 0)
						{
							X1 = random(50, 75); // weight
							X2 = random(7, 11); // STAMIN
							X3 = random(1, 6); // respect
							backLeftBlockSprite.setTextureRect(IntRect(2 * 497, 0, 497, 565));
							dlgtext2 = L"Victory over a weak beast!\nRewards and losses:\nWeight: +" + toString(X1) + L"\nEnergy: -" + toString(X2) + L"\nRespect: -" + toString(X3);
							weight = weight + X1;
							energy = energy - X2;
							respect = respect - X3;
							X9 = 1;
							save();
						}
					}
					else if (questResult < 0)
					{
						if (X9 == 0)
						{
							X1 = random(5, 15); // weight
							X2 = random(14, 22); // STAMIN
							X3 = random(2, 12); // respect
							backLeftBlockSprite.setTextureRect(IntRect(2 * 497, 0, 497, 565));
							dlgtext2 = L"Defeat against a weak beast!\nRewards and losses:\nWeight: +" + toString(X1) + L"\nEnergy: -" + toString(X2) + L"\nRespect: -" + toString(X3);
							weight = weight - X1;
							energy = energy - X2;
							respect = respect - X3;
							X9 = 1;
							save();
						}
					}
				}
				else
				{
					notEnoughEnergySprite.setPosition(450, 250);//450, 250
				}
			}
		}
		else {
			buttonEasyHuntSprite.setTextureRect(IntRect(0 * 139, 0, 139, 175));
			X9 = 0;
		}


		if (buttonHardHuntSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
		{
			buttonHardHuntSprite.setTextureRect(IntRect(139, 0, 139, 175));
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (energy >= 10) {
					questResult = questModul('h');
					if (questResult >= 0) {
						if (X10 == 0)
						{
							backLeftBlockSprite.setTextureRect(IntRect(1 * 497, 0, 497, 565));

							X1 = random(100, 150); // weight
							X2 = random(14, 22); // STAMIN
							X3 = random(7, 11); // respect
							dlgtext2 = L"Victory over a strong beast!\nRewards and losses:\nWeight: +" + toString(X1) + L"\nEnergy: -" + toString(X2) + L"\nRespect: +" + toString(X3);
							weight = weight + X1;
							energy = energy - X2;
							respect = respect + X3;
							X10 = 1;
							save();
						}
					}
					else if (questResult < 0)
					{
						backLeftBlockSprite.setTextureRect(IntRect(1 * 497, 0, 497, 565));

						X1 = random(20, 40); // weight
						X2 = random(12, 18); // STAMIN
						X3 = random(1, 7); // respect
						dlgtext2 = L"Defeat against a strong beast!\nRewards and losses:\nWeight: -" + toString(X1) + L"\nEnergy: -" + toString(X2) + L"\nRespect: +" + toString(X3);
						weight = weight - X1;
						energy = energy - X2;
						respect = respect + X3;
						X10 = 1;
						save();
					}
				}
				else
				{
					notEnoughEnergySprite.setPosition(450, 250);//450, 250
				}
				}
			}
		else {
			buttonHardHuntSprite.setTextureRect(IntRect(0 * 139, 0, 139, 175));
			X10 = 0;
		}


		if (buttonEasyWorkSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
		{
			buttonEasyWorkSprite.setTextureRect(IntRect(139, 0, 139, 175));
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (energy >= 10) {
					if (X11 == 0)
					{
						X1 = random(5, 10); // weight
						X2 = random(7, 11); // STAMIN
						X3 = random(1, 3); // respect
						X4 = random(2, 6); // deep
						dlgtext2 = L"You digging badly, the results are not very good!\nRewards and losses:\nWeight: -" + toString(X1) + L"\nEnergy: -" + toString(X2) + L"\nRespect: -" + toString(X3) + L"\nDeep: +" + toString(X4);
						weight = weight - X1;
						energy = energy - X2;
						respect = respect - X3;
						deep = deep + X4;
						X11 = 1;
						save();

					}
					//backLeftBlockSprite.setTextureRect(IntRect(1 * 497, 0, 497, 565));
				}
				else
				{
					notEnoughEnergySprite.setPosition(450, 250);//450, 250
				}
			}
		}
		else {
			buttonEasyWorkSprite.setTextureRect(IntRect(0 * 139, 0, 139, 175));
			X11 = 0;
		}



		if (buttonHardWorkSprite.getGlobalBounds().contains(pos.x, pos.y))//координата курсора попадает в спрайт
		{
			buttonHardWorkSprite.setTextureRect(IntRect(139, 0, 139, 175));
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (energy >= 10) {
					if (X12 == 0)
					{
						X1 = random(5, 10); // weight
						X2 = random(7, 11); // STAMIN
						X3 = random(1, 3); // respect
						X4 = random(2, 6); // deep
						dlgtext2 = L"You WELL digging, the results are good!\nRewards and losses:\nWeight: -" + toString(X1) + L"\nEnergy: -" + toString(X2) + L"\nRespect: +" + toString(X3) + L"\nDeep: +" + toString(X4);
						weight = weight - X1;
						energy = energy - X2;
						respect = respect - X3;
						deep = deep + X4;
						X12 = 1;
						save();

					}
					//backLeftBlockSprite.setTextureRect(IntRect(1 * 497, 0, 497, 565));
				}
				else
				{
					notEnoughEnergySprite.setPosition(450, 250);//450, 250
				}
			}
		}
		else {
			buttonHardWorkSprite.setTextureRect(IntRect(0 * 139, 0, 139, 175));
			X12 = 0;
		}

		if (energy >= 90 && energy <= 100) energyIconSprite.setTextureRect(IntRect(0 * 130, 0, 130, 134));
		if (energy >= 80 && energy < 90) energyIconSprite.setTextureRect(IntRect(1 * 130, 0, 130, 134));
		if (energy >= 70 && energy < 80) energyIconSprite.setTextureRect(IntRect(2 * 130, 0, 130, 134));
		if (energy >= 60 && energy < 70) energyIconSprite.setTextureRect(IntRect(3 * 130, 0, 130, 134));
		if (energy >= 50 && energy < 60) energyIconSprite.setTextureRect(IntRect(4 * 130, 0, 130, 134));
		if (energy >= 40 && energy < 50) energyIconSprite.setTextureRect(IntRect(5 * 130, 0, 130, 134));
		if (energy >= 30 && energy < 40) energyIconSprite.setTextureRect(IntRect(6 * 130, 0, 130, 134));
		if (energy >= 20 && energy < 30) energyIconSprite.setTextureRect(IntRect(7 * 130, 0, 130, 134));
		if (energy >= 10 && energy < 20) energyIconSprite.setTextureRect(IntRect(8 * 130, 0, 130, 134));
		if (energy >= 0 && energy < 10) energyIconSprite.setTextureRect(IntRect(9 * 130, 0, 130, 134));

		if (respect >= 90 && respect <= 100) respectIconSprite.setTextureRect(IntRect(0 * 130, 0, 130, 134));
		if (respect >= 80 && respect < 90) respectIconSprite.setTextureRect(IntRect(1 * 130, 0, 130, 134));
		if (respect >= 70 && respect < 80) respectIconSprite.setTextureRect(IntRect(2 * 130, 0, 130, 134));
		if (respect >= 60 && respect < 70) respectIconSprite.setTextureRect(IntRect(3 * 130, 0, 130, 134));
		if (respect >= 50 && respect < 60) respectIconSprite.setTextureRect(IntRect(4 * 130, 0, 130, 134));
		if (respect >= 40 && respect < 50) respectIconSprite.setTextureRect(IntRect(5 * 130, 0, 130, 134));
		if (respect >= 30 && respect < 40) respectIconSprite.setTextureRect(IntRect(6 * 130, 0, 130, 134));
		if (respect >= 20 && respect < 30) respectIconSprite.setTextureRect(IntRect(7 * 130, 0, 130, 134));
		if (respect >= 10 && respect < 20) respectIconSprite.setTextureRect(IntRect(8 * 130, 0, 130, 134));
		if (respect >= 0 && respect < 10) respectIconSprite.setTextureRect(IntRect(9 * 130, 0, 130, 134));

		


		window.clear();
		window.draw(oldPaperSprite);
		window.draw(backLeftBlockSprite);
		window.draw(mainBackSprite);
		window.draw(buttonWeightSprite);
		window.draw(buttonCaweightprite);
		window.draw(buttonArenaSprite);
		window.draw(orcSprite);
		window.draw(respectIconSprite);
		window.draw(energyIconSprite);

		window.draw(buttonEasyArenaSprite);
		window.draw(buttonHardArenaSprite);
		window.draw(buttonMainBattleSprite);

		window.draw(buttonEasyHuntSprite);
		window.draw(buttonHardHuntSprite);

		window.draw(buttonEasyWorkSprite);
		window.draw(buttonHardWorkSprite);

		// int energy,deep,weight,respect,scalp;
		//dlgtext2 = "test";
		text.setString(toString(weight)); // weight
		text.setPosition(772, 133);
		text2.setString(toString(deep));  // deep
		text2.setPosition(949, 133);
		text3.setString(toString(scalp));          // arena
		text3.setPosition(1117, 133);
		text4.setString(toString(respect));          // respect
		text4.setPosition(872, 754);
		text5.setString(toString(energy));         //energy
		text5.setPosition(1047, 754);
		window.draw(text);
		window.draw(text2);
		window.draw(text3);
		window.draw(text4);
		window.draw(text5);
		text6.setString(dlgtext);
		text6.setPosition(730, 200);
		text7.setString(dlgtext2);
		text7.setPosition(730, 320);
		window.draw(text6);
		window.draw(text7);
		window.draw(buttonManualSprite);
		window.draw(manualSprite);
		window.draw(buttonBackSprite);
				
		window.draw(weightText);
		window.draw(arenaText);
		window.draw(caveText);
		window.draw(notEnoughEnergySprite);
		
		//if (weight < 10)
		//{
		//	window.draw(GameOverSprite);
		//	window.draw(text8);
		//	text8.setPosition(530, 360);
		//}
		window.display();

	}

	return 0;
}