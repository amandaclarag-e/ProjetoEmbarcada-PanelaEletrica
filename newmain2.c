//Projeto Final
//Amanda Clara Ribeiro Gonçalves
//2020003550

#include "stdio.h"
#include "keypad.h"
#include "bits.h"
#include "config.h"
#include "lcd.h"
#include "io.h"
#include "string.h"
#include "ssd.h"
#include "delay.h"
#include "pwm.h"
#include <pic18f4520.h>

#define CLR 0x01

void escolheTempo(void);
void escolheComida(void);
void abreTampa(void);

void main(void) {
    unsigned int tecla = 16;
    char i;
    float j, k;

    int op = 1;
    while (op == 1) { //Looping para repetir enquanto o usuário não escolher a opção "não"
        
        //início do programa
        atraso_ms(2000);
        char msg[31] = "Qual alimento\nquer cozinhar?"; 
        lcdInit();
        lcdCommand(CLR);
        lcdPosition(0, 0);
        for (i = 0; i < 31; i++) {
            if (strcmp(msg[i], '\n') == 0) {
                lcdCommand(0xC0);
            }
            lcdData(msg[i]);
            if (i == 31) {
                for (k = 0; k < 150; k++);
            }
        }

        //mostra as opções no lcd
        atraso_ms(2000);
        char fr[17] = "1-Frango        ";
        char b[17] = "2-Carne Bovina  ";
        char f[17] = "4-Feijao        ";
        char s[17] = "5-Carne Suina   ";

        for (k = 0; k < 3000; k++);
        lcdCommand(0xF0);
        for (i = 0; i < 17; i++) {
            lcdData(fr[i]);
        }

        for (k = 0; k < 250; k++);
        lcdCommand(0xC0);
        for (i = 0; i < 17; i++) {
            lcdData(b[i]);
        }

        for (k = 0; k < 250; k++);
        lcdCommand(0x90);
        for (i = 0; i < 17; i++) {
            lcdData(f[i]);
        }

        for (k = 0; k < 250; k++);
        lcdCommand(0xD0);
        for (i = 0; i < 17; i++) {
            lcdData(s[i]);
        }

        //chama as funções do programa  
        escolheComida();    //usuário escolhe entre as opções do lcd
        abreTampa();        //animação de abrir a tampa da panela
        escolheTempo();     //escolhe o tempo de cozimento no display 7-seg
        lcdCommand(CLR);

        lcdCommand(CLR);
        
        //ao final das funções aparece um menu se deseja cozinha novamente
        char final[26] = "Deseja cozinhar\nnovamente?";
        lcdPosition(0, 0);
        for (i = 0; i < 26; i++) {
            lcdData(final[i]);
            if (strcmp(final[i], '\n') == 0) {
                lcdCommand(0xC0);
            }
        }
        atraso_ms(2000);
        lcdCommand(CLR);
        char final2[12] = "9-Sim \n#-Nao";
        lcdPosition(0, 0);
        for (i = 0; i < 12; i++) {
            lcdData(final2[i]);
            if (strcmp(final2[i], '\n') == 0) {
                lcdCommand(0xC0);
            }
        }
        //inicialização do teclado para escolha das opções
        kpInit();
        tecla = kpRead();
        int aux3 = 1;
        while (aux3 == 1) {
            kpDebounce();
            if (kpRead() != tecla) {
                tecla = kpRead();
                //se for escolhida a opção sim repete o looping
                if (bitTst(tecla, 9)) {// tecla 9 
                    op = 1;
                    aux3 = 0;
                }
                //se for escolhida a opção não o looping é encerrado
                if (bitTst(tecla, 8)) {// tecla #
                    op = 0;
                    aux3 = 0;
                }
            } 
        }
    }
    //mensagem final caso a opção seja "não"
    char agradece[13] = "Desligando...";
    lcdCommand(CLR);
    lcdPosition(0, 0);
    for (i = 0; i < 13; i++) {
        lcdData(agradece[i]);
        if (strcmp(agradece[i], '\n') == 0) {
            lcdCommand(0xC0);
        }
    }
    for (;;) {
    }
}

void abreTampa(void) {
    char k = 0, i = 0;

    //mensagem no lcd sinalizando a animação
    lcdCommand(CLR);
    atraso_ms(2000);
    char msg[13] = "Abrindo tampa";
    for (i = 0; i < 13; i++) {
        lcdData(msg[i]);
    }
    //inicialização do cooler
    pwmInit();
    pwmSet1(32);
    pwmFrequency(10000);
    atraso_ms(3000); // atraso de 3 segundos
    pwmSet1(0); // desliga o cooler
    pwmFrequency(0);
    lcdCommand(CLR);
    atraso_ms(1000); //atraso de 1 segundo
    //mensagem no lcd
    char msg2[13] = "Insira a agua";
    for (i = 0; i < 13; i++) {
        lcdData(msg2[i]);
    }
    atraso_ms(2000);
    lcdCommand(CLR);
    char msg3[14] = "Fechando tampa";
    for (i = 0; i < 14; i++) {
        lcdData(msg3[i]);
    }
    //inicializa o cooler novamente
    pwmSet1(32);
    pwmFrequency(10000);
    atraso_ms(3000); //após 3 segundos desliga o cooler
    pwmSet1(0);
    pwmFrequency(0);
    atraso_ms(2000);
}

void escolheTempo(void) {
    unsigned int flag = 1;
    int i = 0, k = 0;
    unsigned int tecla = 16;
    long int cont = 0;
    float tempo = 0;
    int aux = 0;
    ssdInit();
    lcdInit();
    //mensagem no lcd para escolher o tempo
    char msg[15] = "Escolha o tempo";
    for (i = 0; i < 15; i++) {
        lcdData(msg[i]);
    }
    atraso_ms(2000);
    lcdCommand(CLR);
    lcdPosition(0, 0); //reposiciona o cursor
    //mensagem digite * para iniciar o contador
    char msg2[21] = "Digite *\npara iniciar";
    for (i = 0; i < 21; i++) {
        lcdData(msg2[i]);
        if (strcmp(msg2[i], '\n') == 0) {
            lcdCommand(0xC0);
        }
    }
    lcdPosition(2, 0);
    //mensagem no lcd para utilização das teclas no temporizador
    char msg3[28] = "4- +1m 5- -1m\n7- +1s  8- -1s";
    for (i = 0; i < 28; i++) {
        lcdData(msg3[i]);
        if (strcmp(msg3[i], '\n') == 0) {
            lcdPosition(3, 0);
        }
    }
    //looping que se repete até que o usuário escolha * (iniciar)
    while (aux != 1) {
        //inicializa o contador
        ssdDigit(((cont / 60000) % 6), 0);
        ssdDigit(((cont / 6000) % 10), 1);
        ssdDigit(((cont / 1000) % 6), 2);
        ssdDigit(((cont / 100) % 10), 3);
        ssdUpdate();
        for (tempo = 0; tempo < 3; tempo++);

        kpInit();
        kpDebounce();
        //configura o teclado para ajustar o tempo no ssd
        if (kpRead() != tecla) {
            tecla = kpRead();
            //aumenta 1 min caso pressione a tecla 4
            if (bitTst(tecla, 2)&& (flag == 0)) {// tecla 4
                cont = cont + 6000;
            }
            //dimnui 1 min caso pressione a tecla 5
            if (bitTst(tecla, 6)&& (flag == 0)) {// tecla 5
                cont = cont - 6000;
            }
            //aumenta 1 seg caso pressione a tecla 7
            if (bitTst(tecla, 1)&& (flag == 0)) {// tecla 7
                cont = cont + 100;
            }
            //dimnui 1 seg caso pressione a tecla 8
            if (bitTst(tecla, 5)&& (flag == 0)) {// tecla 8
                cont = cont - 100;
            }
            //sai do looping que ajusta o tempo e imprime o desenho quando inicia
            if (bitTst(tecla, 0)&& (flag == 0)) {//asterisco
                aux = 1;
                lcdCommand(CLR);
                unsigned char foguinho[64] = {
                    0x00, 0x00, 0x02, 0x02, 0x06, 0x06, 0x02, 0x02,
                    0x08, 0x0C, 0x0A, 0x09, 0x05, 0x15, 0x19, 0x0A,
                    0x00, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00,
                    0x02, 0x02, 0x06, 0x0A, 0x09, 0x08, 0x06, 0x01,
                    0x0A, 0x0A, 0x09, 0x11, 0x11, 0x00, 0x00, 0x1F,
                    0x04, 0x04, 0x04, 0x0A, 0x12, 0x02, 0x04, 0x18
                };


                lcdCommand(CLR);
                lcdCommand(0x40);
                for (i = 0; i <= 64; i++) {
                    lcdData(foguinho[i]);
                }
                lcdPosition(1, 6);
                lcdData(0);
                lcdData(1);
                lcdData(2);
                lcdPosition(2, 6);
                lcdData(3);
                lcdData(4);
                lcdData(5);
            }
        }
        flag = 0;
    }
    
    //contagem regressiva enquanto o contador for maior que 0
    while (cont > 0) {
        //inicia o cooler (panela)
        pwmSet1(32);
        pwmFrequency(10000);
        
        //contagem regressiva no ssd
        kpDebounce();
        cont--;
        ssdDigit(((cont / 60000) % 6), 0);
        ssdDigit(((cont / 6000) % 10), 1);
        ssdDigit(((cont / 1000) % 6), 2);
        ssdDigit(((cont / 100) % 10), 3);
        ssdUpdate();
        for (tempo = 0; tempo < 15; tempo++);

        //repete o código de cima para ajuste do tempo durante a contagem
        if (kpRead() != tecla) {
            tecla = kpRead();

            if (bitTst(tecla, 2)) {// tecla 4
                cont = cont + 6000;
            }
            if (bitTst(tecla, 6)) {// tecla 5
                cont = cont - 6000;
            }
            if (bitTst(tecla, 1)) {// tecla 7
                cont = cont + 100;
            }
            if (bitTst(tecla, 5)) {// tecla 8
                cont = cont - 100;
            }
        }
    }
    
    //quando o contador chega a 0 (acaba a contagem)
    if (cont == 0) {
        //desliga o cooler
        lcdCommand(CLR);
        pwmSet1(0);
        pwmFrequency(0);
        
        //ativa o buzzer
        for (i = 0; i < 10; i++) {
            bitSet(TRISC, 1);
            PORTB = i;
            PORTD = i;
            atraso_ms(50); // duração do buzzer
        }
        bitClr(TRISC, 1);


        PORTB = 0;
        PORTD = 0;
        lcdCommand(CLR); // limpa o lcd
    }

}

void escolheComida(void) {
    unsigned int tecla = 16;
    char i = 0, k = 0;
    int aux = 1;
    unsigned int flag = 1;
    kpInit();
    tecla = kpRead();
    //looping que se repete enquanto o usuário nao escolher uma opcao
    while (aux != 0) {
        kpDebounce();
        //configuração do teclado para a escolha da opcao de comida
        if (tecla != kpRead) {
            tecla = kpRead();
            if (bitTst(tecla, 3)&& (flag == 0)) { // escolhe frango se presionar a tecla 1
                lcdCommand(CLR);
                lcdCommand(0xF0);
                //digita no lcd a opcao escolhida
                char msg3[19] = "Escolhido:\nFrango";
                for (i = 0; i < 19; i++) {
                    lcdData(msg3[i]);
                    if (strcmp(msg3[i], '\n') == 0) {
                        lcdCommand(0xC0);
                    }

                }
                //espera 2 segundos e limpa o lcd
                atraso_ms(2000);
                lcdCommand(CLR);
                lcdPosition(0, 0);
                aux = 0; // sai do looping após esoclher uma opção
            }
            if (bitTst(tecla, 7)&& (flag == 0)) { // escolhe Carne Bovina se presionar a tecla 2

                lcdCommand(CLR);
                lcdCommand(0xF0);
                //digita no lcd a opcao escolhida
                char msg3[24] = "Escolhido:\nCarne Bovina";
                for (i = 0; i < 24; i++) {
                    lcdData(msg3[i]);
                    if (strcmp(msg3[i], '\n') == 0) {
                        lcdCommand(0xC0);
                    }

                }
                //espera 2 segundos e limpa o lcd
                atraso_ms(2000);
                lcdCommand(CLR);
                lcdPosition(0, 0);
                aux = 0;    // sai do looping após esoclher uma opção

            }
            if (bitTst(tecla, 2)&& (flag == 0)) { // escolhe Feijao se presionar a tecla 4
                //digita no lcd a opcao escolhida
                lcdCommand(CLR);
                lcdCommand(0xF0);
                char msg3[19] = "Escolhido:\nFeijao";
                for (i = 0; i < 19; i++) {
                    lcdData(msg3[i]);
                    if (strcmp(msg3[i], '\n') == 0) {
                        lcdCommand(0xC0);
                    }
                }
                //espera 2 segundos e limpa o lcd
                atraso_ms(2000);
                lcdCommand(CLR);
                lcdPosition(0, 0);
                aux = 0;    // sai do looping após esoclher uma opção
            }
            if (bitTst(tecla, 6)&& (flag == 0)) { // escolhe Carne Suina se presionar a tecla 5

                lcdCommand(CLR);
                lcdCommand(0xF0);
                //digita no lcd a opcao escolhida
                char msg3[23] = "Escolhido:\nCarne Suina";
                for (i = 0; i < 23; i++) {
                    lcdData(msg3[i]);
                    if (strcmp(msg3[i], '\n') == 0) {
                        lcdCommand(0xC0);
                    }

                }
                //espera 2 segundos e limpa o lcd
                atraso_ms(2000);
                lcdCommand(CLR);
                lcdPosition(0, 0);
                aux = 0;    // sai do looping após esoclher uma opção

            }

        }
        flag = 0;
    }

}


