/*
Name:		algArduino.ino
Created:	5/24/2017 11:59:12 PM
Author:	Matheus Rossetti & Rian Turibio
*/
#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>

#define MAX_VEL 255 //Velocidade Maxima
#define MIN_VEL 10  //Velocidade Minima

//Passo 1 - Instanciando um objeto da biblioteca
Fuzzy* fuzzy = new Fuzzy();

//PINAGEM MOTORES
const int MOTOR_1_A = 11;
const int MOTOR_1_B = 10;
const int MOTOR_2_A = 9;
const int MOTOR_2_B = 6;
const int MOTOR_1_Enable = 5;
const int MOTOR_2_Enable = 4;
//PINAGEM SENSORES
const int Trigger_1 = 13;
const int Echo_1 = 12;
const int Trigger_2 = 3;
const int Echo_2 = 2;



//Movimenta��es
void  movTras()
{
	digitalWrite(MOTOR_1_B, LOW);
	digitalWrite(MOTOR_2_B, LOW);
	delay(50);
	analogWrite(MOTOR_1_B, MAX_VEL);
	analogWrite(MOTOR_2_B, MAX_VEL);
}
void  movFrente()
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, MAX_VEL);
	analogWrite(MOTOR_2_A, MAX_VEL);
}
void  movDireita()
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, 0);
	analogWrite(MOTOR_2_A, MAX_VEL);
}
void  movEsquerda()
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, MAX_VEL);
	analogWrite(MOTOR_2_A, 0);
}
//Suaviza��o de Movimentos Curvos
void  curvaEsquerda()
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, MIN_VEL);
	analogWrite(MOTOR_2_A, MAX_VEL);
}
void  curvaDireita()
{
	digitalWrite(MOTOR_1_A, LOW);
	digitalWrite(MOTOR_2_A, LOW);
	delay(50);
	analogWrite(MOTOR_1_A, MAX_VEL);
	analogWrite(MOTOR_2_A, MIN_VEL);
}
//Leitura dos Sensores
float lerSensor_1()
{
	float vlr_lido;
	digitalWrite(Trigger_1, HIGH);
	delayMicroseconds(50);
	digitalWrite(Trigger_1, LOW);
	vlr_lido = pulseIn(Echo_1, HIGH);												//Tempo do Sinal

	return (vlr_lido / 2 / 29.1);														//Em Cm.
}
float lerSensor_2()
{
	float vlr_lido;
	digitalWrite(Trigger_2, HIGH);
	delayMicroseconds(50);
	digitalWrite(Trigger_2, LOW);
	vlr_lido = pulseIn(Echo_2, HIGH);												//Tempo do Sinal

	return (vlr_lido / 2 / 29.1);														//Em Cm.
}
//Implementa��o eFLL
void  programa_1()
{
int Dist1, Dist2;

  Dist1 = lerSensor_1();
  Dist2 = lerSensor_2();

  //Fuzzyfica e Defuzzyfica Sensor 1  
  fuzzy->setInput(1, Dist1);
  fuzzy->fuzzify();
  float output1 = fuzzy->defuzzify(1);

  //Fuzzyfica e Defuzzyfica Sensor 2 
  fuzzy->setInput(2, Dist2);
  fuzzy->fuzzify();
  float output2 = fuzzy->defuzzify(2);


  Serial.print("Saida Defuzzy 1: ");
  Serial.print(output1);
  Serial.print("  Saida Defuzzy 2: ");
  Serial.println(output2);
  
 if (output1 == output2){
       movFrente();
  }else if(output1 > output2){
       movTras();
       delay(1000);
       movDireita();
  }else if(output1 < output2){
       movTras();
       delay(1000);
       movEsquerda();
  }else if(output2 > output1){
       movTras();
       delay(1000);
       movEsquerda();
  }else if(output2 < output1){
       movTras();
       delay(1000);
       movDireita();
  }

  // wait 100 milli seconds before looping again
  delay(100);
}


//Implementa��o MLP
void  programa_2()
{

}

//A fun��o de configura��o � executada uma vez quando voc� pressiona reset ou liga a placa
void setup         ()
{
	Serial.begin(9600);
	void  movTras();
	void  movFrente();
	void  movEsquerda();
	void  movDireita();
	void  curvaDireita();
	void  curvaEsquerda();

	float lerSensor_1();
	float lerSensor_2();
	void  programa_1();
	void  programa_2();

	//Modos dos Pinos dos Motores
	pinMode(MOTOR_1_A, OUTPUT);
	pinMode(MOTOR_1_B, OUTPUT);
	pinMode(MOTOR_2_A, OUTPUT);
	pinMode(MOTOR_2_B, OUTPUT);
	pinMode(MOTOR_1_Enable, OUTPUT);
	pinMode(MOTOR_2_Enable, OUTPUT);
	//Modos dos Pinos dos Sensores
	pinMode(Trigger_1, OUTPUT);
	pinMode(Echo_1, INPUT);
	pinMode(Trigger_2, OUTPUT);
	pinMode(Echo_2, INPUT);
	//Ligar Motores
	digitalWrite(MOTOR_1_Enable, HIGH);
	digitalWrite(MOTOR_2_Enable, HIGH);




	//UTILIZA��O BIBLIOTECA eFLL
										//Passo 2 - Criando o FuzzyInput distancia
	FuzzyInput* distancia = new FuzzyInput(1);												// Como parametro seu ID
																							// Criando os FuzzySet que compoem o FuzzyInput distancia
	FuzzySet* proximo = new FuzzySet(2, 5, 5, 8);											// Distancia pequena
		distancia->addFuzzySet(proximo);													// Adicionando o FuzzySet small em distance
	FuzzySet* seguro = new FuzzySet(10, 13, 13, 17);										// Distancia segura
		distancia->addFuzzySet(seguro);														// Adicionando o FuzzySet safe em distance
	FuzzySet* distante = new FuzzySet(15, 21, 21, 24);										// Distancia grande
		distancia->addFuzzySet(distante);													// Adicionando o FuzzySet big em distance
	fuzzy->addFuzzyInput(distancia);														// Adicionando o FuzzyInput no objeto Fuzzy

										// Passo 3 - Criando o FuzzyOutput velocidade
	FuzzyOutput* sentido = new FuzzyOutput(1);												// Como parametro seu ID
																							// Criando os FuzzySet que compoem o FuzzyOutput velocidade
	FuzzySet* sentidoParado   = new FuzzySet(0, 10, 10, 20);								// Velocidade parado
		sentido->addFuzzySet(sentidoParado);												// Adicionando o FuzzySet slow em velocity
	FuzzySet* sentidoDireita  = new FuzzySet(255, 255, 255, 255);							// Velocidade para direita
		sentido->addFuzzySet(sentidoDireita);												// Adicionando o FuzzySet average em velocity
	FuzzySet* sentidoEsquerda = new FuzzySet(255, 255, 255, 255);							// Velocidade para esquerda
		sentido->addFuzzySet(sentidoEsquerda);												// Adicionando o FuzzySet average em velocity
	FuzzySet* sentidoFrente   = new FuzzySet(60, 80, 80, 100);								// Velocidade alta para frente
		sentido->addFuzzySet(sentidoEsquerda);												// Adicionando o FuzzySet fast em velocity
	FuzzySet* sentidoTras     = new FuzzySet(60, 80, 80, 100);								// Velocidade alta para tras
		sentido->addFuzzySet(sentidoTras);													// Adicionando o FuzzySet fast em velocity
	fuzzy->addFuzzyOutput(sentido);															// Adicionando o FuzzyOutput no objeto Fuzzy

										//Passo 4 - Montando as regras Fuzzy
																							// FuzzyRule "SE distancia = pequena ENTAO velocidade = lenta"
	FuzzyRuleAntecedent* ifPertoD_PertoE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifPertoD_PertoE->joinWithAND(proximo, proximo);										// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenPara = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenPara->addOutput(sentidoParado);													// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifPertoD_PertoE, thenPara);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule01);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//---------------------  
																							// FuzzyRule "SE distancia = pequena ENTAO velocidade = lenta"
	FuzzyRuleAntecedent* ifPertoD_MedioE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifPertoD_MedioE->joinWithAND(proximo, seguro);										// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVDir1 = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenVDir1->addOutput(sentidoDireita);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifPertoD_MedioE, thenVDir1);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule02);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//--------------------- 																							// FuzzyRule "SE distancia = pequena ENTAO velocidade = lenta"																							
	FuzzyRuleAntecedent* ifPertoD_LongeE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifPertoD_LongeE->joinWithAND(proximo, distante);									// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVDir2 = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenVDir2->addOutput(sentidoDireita);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifPertoD_LongeE, thenVDir2);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule03);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//--------------------- 
																							// FuzzyRule "SE distancia = pequena ENTAO velocidade = lenta"
	FuzzyRuleAntecedent* ifMedioD_PertoE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifMedioD_PertoE->joinWithAND(seguro, proximo);										// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVEsq1 = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenVEsq1->addOutput(sentidoEsquerda);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule04 = new FuzzyRule(4, ifMedioD_PertoE, thenVEsq1);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule04);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//--------------------- 
																							// FuzzyRule "SE distancia = pequena ENTAO velocidade = lenta"
	FuzzyRuleAntecedent* ifMedioD_MedioE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifMedioD_MedioE->joinWithAND(seguro, seguro);										// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenReto = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenReto->addOutput(sentidoFrente);													// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule06 = new FuzzyRule(6, ifMedioD_MedioE, thenReto);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule06);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//--------------------- 
																							// FuzzyRule "SE distancia = pequena ENTAO velocidade = lenta"
	FuzzyRuleAntecedent* ifMedioD_LongeE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifMedioD_LongeE->joinWithAND(seguro, distante);										// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVDir3 = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenVDir3->addOutput(sentidoDireita);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule07 = new FuzzyRule(7, ifMedioD_LongeE, thenVDir3);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule07);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//--------------------- 
																							// FuzzyRule "SE distancia = pequena ENTAO velocidade = lenta"
	FuzzyRuleAntecedent* ifLongeD_PertoE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifLongeD_PertoE->joinWithAND(distante, proximo);									// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVEsq2 = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenVEsq2->addOutput(sentidoEsquerda);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule05 = new FuzzyRule(5, ifLongeD_PertoE, thenVEsq2);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule05);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//---------------------
																							// FuzzyRule "SE distancia = pequena ENTAO velocidade = lenta"
	FuzzyRuleAntecedent* ifLongeD_MedioE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifLongeD_MedioE->joinWithAND(distante, seguro);										// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVEsq4 = new FuzzyRuleConsequent();								// Instancinado um Consequente para a expressao
		thenVEsq4->addOutput(sentidoEsquerda);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule08 = new FuzzyRule(8, ifLongeD_MedioE, thenVEsq4);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule08);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//---------------------
																							// FuzzyRule "SE distancia = pequena ENTAO velocidade = lenta"
	FuzzyRuleAntecedent* ifLongeD_LongeE = new FuzzyRuleAntecedent();						// Instanciando um Antecedente para a expresso
		ifLongeD_LongeE->joinWithAND(distante, distante);									// Adicionando o FuzzySet correspondente ao objeto Antecedente
	FuzzyRuleConsequent* thenVReto2 = new FuzzyRuleConsequent();							// Instancinado um Consequente para a expressao
		thenVReto2->addOutput(sentidoFrente);												// Adicionando o FuzzySet correspondente ao objeto Consequente
																							// Instanciando um objeto FuzzyRule
	FuzzyRule* fuzzyRule09 = new FuzzyRule(9, ifLongeD_LongeE, thenVReto2);					// Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule09);													// Adicionando o FuzzyRule ao objeto Fuzzy
	//---------------------
}

//A fun��o de loop � executada repetidamente at� que a alimenta��o seja desligada ou reinicializada
void loop() 
{
	programa_1();
}
