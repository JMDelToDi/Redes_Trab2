//------------------------------------------------------------------------------//
// SSC0641 - REDES DE COMPUTADORES - TRABALHO 2 - SIMULACAO DA CAMADA DE ENLACE //
//------------------------------------------------------------------------------//
// Antonio Sebastian Fernandes Rabelo - NUSP 10797781                           //
// Joao Marcos Della Torre Divino - NUSP 10377708                               //
// Paulo Inay Cruz - NUSP 10388691                                              //
// Vitor Rodrigues Leonardi - NUSP 10716567                                     //
// Vitor Oliveira Caires - NUSP 10748027                                        //
//------------------------------------------------------------------------------//

// Bibliotecas utilizadas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Defines
#define MsgLen 256
#define CRC 32
#define msgbitlen (MsgLen * 8)
#define tipoControle 0              // Define para o tipo de controle a ser realizado

// Variaveis globais
char Mensagem[MsgLen];
int MensagemBit[msgbitlen + CRC];
const int crc32[] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1};

//------------------------------------------------------------------------------//
//        DECLARACAO DAS FUNCOES IMPLEMENTADAS REPRESENTANDO AS CAMADAS         //
//------------------------------------------------------------------------------//

//                           Funcoes do transmissor                             //
void AplicacaoTransmissora();
void CamadaDeAplicacaoTransmissora(char *Mensagem);
void CamadaEnlaceDadosTransmissora(int quadro[]);
void CamadaEnlaceDadosTransmissoraControleDeErro(int quadro[]);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(int quadro[]);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(int quadro[]);
void CamadaEnlaceDadosTransmissoraControleDeErroCRC(int quadro[]);


//                       Funcao do meio de comunicacao                          //
void MeioDeComunicacao(int fluxoBrutoDeBitsPontoA[]);


//                            Funcoes do receptor                               //
void CamadaEnlaceDadosReceptora(int quadro[]);
void CamadaEnlaceDadosReceptoraControleDeErro(int quadro[]);
void CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(int quadro[]);
void CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(int quadro[]);
void CamadaEnlaceDadosReceptoraControleDeErroCRC(int quadro[]);
void CamadaDeAplicacaoReceptora(int quadro[]);
void AplicacaoReceptora(char *Mensagem);

//------------------------------------------------------------------------------//
//                                    MAIN                                      //
//------------------------------------------------------------------------------//

void main(void){
    
    printf(">>> SIMULADOR DA CAMADA DE ENLACE <<<\n\n");
    
    // Fluxo principal
    AplicacaoTransmissora();                        // Recebe a mensagem e converte para bit (char to bit)
    CamadaEnlaceDadosTransmissora(MensagemBit);     // Aplicar os codigos de verificacao de erro
    MeioDeComunicacao(MensagemBit);                 // Gera erros randomicos na mensagem
    CamadaEnlaceDadosReceptora(MensagemBit);        // Verifica se a mensagem veio com erro
    CamadaDeAplicacaoReceptora(MensagemBit);        // Recompoe a mensagem (bit to char)
    
    return;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * Transmissor * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//------------------------------------------------------------------------------//
//                           APLICACAO TRANSMISSORA                             //
//------------------------------------------------------------------------------//
void AplicacaoTransmissora(){                       // Obtem a mensagem da aplicacao, e transfere para a camada de baixo
    
    printf("--> Digite sua mensagem: \n");          //
    memset(Mensagem,'\0',sizeof(Mensagem));         //  Recebe a mensagem do usuario
    fgets(Mensagem, 256, stdin);                    //
    
    CamadaDeAplicacaoTransmissora(Mensagem);        // Envia a mensagem para a camada de baixo
    
    return;

}

//------------------------------------------------------------------------------//
//                       CAMADA DE APLICACAO TRANSMISSORA                       //
//------------------------------------------------------------------------------//
void CamadaDeAplicacaoTransmissora(char *Mensagem){ // Recebe a mensagem e converte para bit

    int ascii = 0, i = 0, j = 0, k = 0;
    int aux[8];
    
    memset(MensagemBit,'\0',sizeof(MensagemBit));

    // Conversao char para bit
    for(i = 0; i < strlen(Mensagem); i++){
        
        // Char para int
        ascii = (int) Mensagem[i];
	
        // Int para bit
        for (j = 7; j > 0; j--){
			aux[j] = ascii % 2;
            ascii /= 2;
		}
        aux[0] = ascii;

        // Transcricao para o vetor da mensagem em bits
        for (j = 0; j < 8; j++){
            MensagemBit[k] = aux[j];
            k++;
        }

    }

    return;
}

//------------------------------------------------------------------------------//
//                   CAMADA DE ENLACE DE DADOS TRANSMISSORA                     //
//------------------------------------------------------------------------------//
void CamadaEnlaceDadosTransmissora(int quadro[]){
    
    // Chama funcao para fazer o controle dos erros
    CamadaEnlaceDadosTransmissoraControleDeErro(quadro);
    
    return;

}

//------------------------------------------------------------------------------//
//          CONTROLE DE ERROS CAMADA DE ENLACE DE DADOS TRANSMISSORA            //
//------------------------------------------------------------------------------//
void CamadaEnlaceDadosTransmissoraControleDeErro(int quadro[]){
    
    int tipoDeControleDeErro = tipoControle; // Alterar de acordo com o tipo a ser realizado

    switch (tipoDeControleDeErro){ // Selecao do controle de erros

        case 0: // Bit de paridade par
            CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(quadro);
            break;
        
        case 1: // Bit de paridade impar
            CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(quadro);
            break;
        
        case 2: // CRC
            CamadaEnlaceDadosTransmissoraControleDeErroCRC(quadro);
            break;

    }

}

//------------------------------------------------------------------------------//
//                   CONTROLE DE ERRO - BIT DE PARIDADE PAR                     //
//------------------------------------------------------------------------------//
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(int quadro[]){
    
    int sum = 0; // Bit sum se torna 0 toda vez que encontra um bit igual a ele
                 // e 1 toda vez que o bit for diferente (XOR)
                 // Neste caso, bit inicial = 0 verifica se ha um numero par
                 // de 1s na mensagem. Se no final o bit for 0, o numero eh par
                 // caso contrario, ele eh impar. Se for impar, deve ser adicio-
                 // nado 1 ao final (de forma a tornar a paridade da mensagem par)

    printf("Bits pre-paridade par:\n");             //
    for(int i = 0; i < msgbitlen + CRC; i++)        // Impressao para pre-avaliacao
        printf("%d", quadro[i]);                    //
    
    printf("\n");

    for(int i = 0; i < msgbitlen; i++){
        
        if((sum == 0 && quadro[i] == 0) || (sum == 1 && quadro[i] == 1))
            sum = 0;
        else
            sum = 1;

    }
    quadro[msgbitlen] = sum;

    printf("Bits pos-paridade par:\n");             //
    for(int i = 0; i < msgbitlen + CRC; i++)        // Impressao para avaliacao do teste
        printf("%d", quadro[i]);                    //
    
    printf("\n");
    
    return;
}

//------------------------------------------------------------------------------//
//                  CONTROLE DE ERRO - BIT DE PARIDADE IMPAR                    //
//------------------------------------------------------------------------------//
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(int quadro[]){
    
    int sum = 1; // Semelhante ao teste de paridade par, mas o bit inicial eh 1.
                 // Bit final = 0 => numero impar de 1s, 0 eh adicionado no final.
                 // Bit final = 1 => numero par de 1s, 1 eh adicionado no final
                 //(de forma a tornar a paridade da mensagem impar)

    printf("Bits pre-paridade impar:\n");           //
    for(int i = 0; i < msgbitlen + CRC; i++)        // Impressao para pre-avaliacao
        printf("%d", quadro[i]);                    //
    
    printf("\n");

    for(int i = 0; i < msgbitlen; i++){
        
        if((sum == 0 && quadro[i] == 0) || (sum == 1 && quadro[i] == 1))
            sum = 0;
        else
            sum = 1;

    }
    quadro[msgbitlen] = sum;

    printf("Bits pos-paridade impar:\n");           //
    for(int i = 0; i < msgbitlen + CRC; i++)        // Impressao para avaliacao do teste
        printf("%d", quadro[i]);                    //
    
    printf("\n");

    return;
}

//------------------------------------------------------------------------------//
//                          CONTROLE DE ERRO - CRC                              //
//------------------------------------------------------------------------------//
void CamadaEnlaceDadosTransmissoraControleDeErroCRC(int quadro[]){ //Polinomio CRC-32 (IEEE 802)

    // Variavel auxiliar
    int QuadroTemp[msgbitlen + CRC];

    
	for (int i = 0; i < msgbitlen + CRC; i++){
        QuadroTemp[i] = quadro[i];
    }

    printf("Bits pre-CRC:\n");                      //
    for(int i = 0; i < msgbitlen + CRC; i++)        // Impressao para pre-avaliacao
        printf("%d", QuadroTemp[i]);                //
    
    printf("\n");
  

	for (int i = 0; i < msgbitlen; i++){

		if (QuadroTemp[i] == 0)
			continue;

		for (int j = 0; j < CRC; j++){

			if (QuadroTemp[i + j] == crc32[j])
                QuadroTemp[i + j] = 0;
            else
                QuadroTemp[i + j] = 1;

		}

	}

    printf("Bits pos-CRC:\n");                      //
    for(int i = 0; i < msgbitlen + CRC; i++)        // Impressao para avaliacao do teste
        printf("%d", QuadroTemp[i]);                //
    
    printf("\n");

	for (int k = 0; k < CRC; k++)
		quadro[msgbitlen + k] = QuadroTemp[msgbitlen + k];

    return;

}

    //--------------------------------------------- ALGORITMO - CRC --------------------------------------------//
    //                                                                                                          //
    //    111010101110100110101010101010111110110101010101010 + 00000000000000000 (dados + 0000)                //
    //    bitwise XOR                                                                                           //
    //    10000010011000000010001010110110111 crc 32                                                            //
    //    --------------------------------------------------------------------------------------------------    //
    //    01101000100010..........                                                                              //
    //    XOR                                                                                                   //
    //    crc 32                                                                                                //
    //                                                                                                          //
    //    0010101010101.....                                                                                    //
    //    XOR                                                                                                   //
    //    crc32                                                                                                 //
    //                                                                                                          //
    //    ... MsgLen * 8 iteracoes                                                                              //
    //                                                                                                          //
    //    0000000000000000000000000000000000000000000000000000 1100101010101011010 (32 bits) resto da divisao   //
    //                                                                                                          //
    //    ENVIAMOS PARA O RECPTOR A MENSAGEM MAIS O RESTO DA DIVISAO:                                           //
    //    111010101110100110101010101010111110110101010101010 1100101010101011010 (msg + resto)                 //
    //                                                                                                          //
    //----------------------------------------------------------------------------------------------------------//

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/* * * * * * * * * * * * * * * Fim do Transmissor * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/* * * * * * * * * * * * * *  Meio de Comunicacao * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
void MeioDeComunicacao(int fluxoBrutoDeBitsPontoA[]){
    
    int erro, porcentagemDeErros;
    int fluxoBrutoDeBitsPontoB[msgbitlen + CRC] ;
    int i = 0;

    memset(fluxoBrutoDeBitsPontoB,'\0',sizeof(fluxoBrutoDeBitsPontoB));

    // Alterar de acordo com a porcentagem de erro desejada
    porcentagemDeErros = 0; //10%, 20%, 30%, 40%, ..., 100%

    for(i = 0; i < msgbitlen + CRC; i++){

        if((rand()%100) >= porcentagemDeErros) // Fazer a probabilidade do erro
            fluxoBrutoDeBitsPontoB[i] += fluxoBrutoDeBitsPontoA[i];
        else{ // ERRO! INVERTER (usa condicao ternaria)

            if(!fluxoBrutoDeBitsPontoB[i])
                fluxoBrutoDeBitsPontoA[i] = (fluxoBrutoDeBitsPontoB[i] + 1);
            else
                fluxoBrutoDeBitsPontoA[i] = (fluxoBrutoDeBitsPontoB[i] - 1);

        }

    }
    
    return;

}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/* * * * * * * * * * * * * Fim do Meio de Comunicacao * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/* * * * * * * * * * * * * * * * * * Receptor * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

//------------------------------------------------------------------------------//
//                      CAMADA DE ENLACE DE DADOS TRANSMISSORA                  //
//------------------------------------------------------------------------------//
void CamadaEnlaceDadosReceptora(int quadro[]){
    
    // Chama funcao para fazer o controle dos erros
    CamadaEnlaceDadosReceptoraControleDeErro(quadro);
    
    return;

}

//------------------------------------------------------------------------------//
//             CONTROLE DE ERROS CAMADA DE ENLACE DE DADOS RECEPTORA            //
//------------------------------------------------------------------------------//
void CamadaEnlaceDadosReceptoraControleDeErro(int quadro[]){
    
    int tipoDeControleDeErro = tipoControle; // Alterar de acordo com o tipo a ser realizado

    switch (tipoDeControleDeErro){ // Selecao do controle de erro
        
        case 0: // Bit de paridade par
            CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(quadro);
            break;
        
        case 1: // Bit de paridade impar
            CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(quadro);
            break;
        
        case 2: // CRC
            CamadaEnlaceDadosReceptoraControleDeErroCRC(quadro);
            break;
    
    }

}


//------------------------------------------------------------------------------//
//                 CONTROLE DE ERRO - TESTE DE PARIDADE PAR                     //
//------------------------------------------------------------------------------//
void CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(int quadro[]){    

    int sum = 0; // Como explicado no transmissor, se bit final = 0, a mensagem possui
                 // paridade par, e portanto ou nao ocorreram erros ou ocorreram um numero
                 // par de erros.

    printf("Bits apos meio:\n");                    //
    for(int i = 0; i < msgbitlen + CRC; i++)        // Impressao para pre-avaliacao
        printf("%d", quadro[i]);                    //
    
    printf("\n");

    for(int i = 0; i < msgbitlen + CRC; i++){
        
        if((sum == 0 && quadro[i] == 0) || (sum == 1 && quadro[i] == 1))
            sum = 0;
        else
            sum = 1;

    }

    if(sum) //OCORREU NUMERO IMPAR DE ERROS
        printf("[ERRO] Ocorreu um erro! Paridade invalida;\nMensagem descartada\n");
    else //NAO OCORREU ERROS OU OCORREU NUMERO PAR DE ERROS
        printf("Mensagem recebida normalmente\n");

    return;

}

//------------------------------------------------------------------------------//
//                CONTROLE DE ERRO - TESTE DE PARIDADE IMPAR                    //
//------------------------------------------------------------------------------//
void CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(int quadro[]){
    
    int sum = 1; // Como explicado no transmissor, se bit final = 0, a mensagem possui
                 // paridade impar, e portanto ou nao ocorreram erros ou ocorreram um numero
                 // par de erros.

    printf("Bits apos meio:\n");                    //
    for(int i = 0; i < msgbitlen + 1; i++)          // Impressao para pre-avaliacao
        printf("%d", quadro[i]);                    //
    
    printf("\n");
    
    for(int i = 0; i < msgbitlen + 1; i++){
        
        if((sum == 0 && quadro[i] == 0) || (sum == 1 && quadro[i] == 1))
            sum = 0;
        else
            sum = 1;

    }

    if(sum) //OCORREU NUMERO IMPAR DE ERROS
        printf("[ERRO] Ocorreu um erro! Paridade invalida;\nMensagem descartada\n");
    else //NAO OCORREU ERROS OU OCORREU NUMERO PAR DE ERROS
        printf("Mensagem recebida normalmente\n");

    return;

}


//------------------------------------------------------------------------------//
//                      CONTROLE DE ERRO - CHECAGEM CRC                         //
//------------------------------------------------------------------------------//
void CamadaEnlaceDadosReceptoraControleDeErroCRC(int quadro[]){ //Polinomio CRC-32 (IEEE 802)
	
    int QuadroTemp[msgbitlen + CRC];
    int flag_error = 0;

	for (int i = 0; i < msgbitlen + CRC; i++){
        QuadroTemp[i] = quadro[i];
    }

    printf("Bits apos meio:\n");                    //
    for(int i = 0; i < msgbitlen + CRC; i++)        // Impressao para pre-avaliacao
        printf("%d", QuadroTemp[i]);                //
    
    printf("\n");

	for (int i = 0; i < msgbitlen; i++){

		if (QuadroTemp[i] == 0)
			continue;

		for (int j = 0; j < CRC; j++){

			if (QuadroTemp[i + j] == crc32[j])
                QuadroTemp[i + j] = 0;
            else
                QuadroTemp[i + j] = 1;

		}

	}

	for (int k = 0; k < CRC; k++){
        
        if(QuadroTemp[msgbitlen + k] == 1){
            flag_error = 1;
            break;
        }

    }

    printf("Bits pos divisao:\n");                  //
    for(int i = 0; i < msgbitlen + CRC; i++)        // Impressao para avaliacao do teste
        printf("%d", QuadroTemp[i]);                //
    
    printf("\n");

    if(flag_error)
        printf("[ERRO] Ocorreu um erro! Codigo CRC invalido;\nMensagem descartada\n");
    else
        printf("Mensagem recebida normalmente\n");
	
    return;

}

//------------------------------------------------------------------------------//
//                      CAMADA DE APLICACAO RECEPTORA                           //
//------------------------------------------------------------------------------//
void CamadaDeAplicacaoReceptora(int quadro[]){
    
    int asciiTemp = 0;
    int i = 0, j = 0;

    // Converte de Bit a Char
    for(i = 0; i < strlen(Mensagem); i++){
        
        // Binário para int
		for(j = 0; j < 8; j++){
            asciiTemp += pow(2, 7 - j) * quadro[((i*8) + j)]; // (7 - j), pois o mais significativo esta a esquerda
        }
        
        // Int para char
        Mensagem[i] = (char) asciiTemp;
        asciiTemp = 0;

    }

    // Envia mensagem para a aplicacao (camada superior)
    AplicacaoReceptora(Mensagem);

    return;

}

//------------------------------------------------------------------------------//
//                              APLICACAO RECEPTORA                             //
//------------------------------------------------------------------------------//
void AplicacaoReceptora(char *Mensagem){
    
    // Print final da mensagem recebida
    printf("\nMensagem recebida: %s\n",Mensagem);
    
    return;

}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * Fim do Receptor * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */