/*
 * bp_helper.h
 * Computer Architecture - Lab 7: Branch Prediction
 * Derek Chiou
 * Alex Hsu, Chirag Sakhuja, Tommy Huynh
 * Spring 2016
 *
 * This file contains any common helper functions you might need.
 * YOU CAN CHANGE ANYTHING IN THIS FILE.
 */

#ifndef _BP_HELPER_H_
#define _BP_HELPER_H_
#include <vector>
#include <cmath>

class BHR
{
public:
    unsigned int conteudo, mascara;

    BHR(){}

    BHR(int k)
    {
        conteudo = 0;
        mascara = (int)(pow(2.0, (double)k) - 1);
    }

    void atualiza(bool tomado)
    {
        // cout << " BHR pre atualiza|" << conteudo << endl;
        conteudo = conteudo << 1;
        if (tomado)
        {
            conteudo++;
        }
        conteudo = conteudo & mascara;
        // cout << " BHR pos atualiza|" << conteudo << endl;

    }
};

class PHT
{
public:
    // Vetor com os estados de todos os contadores saturados.
    vector<int> contSaturado;

    PHT(){}

    PHT(int n)
    {
        int tamanho = (int)pow(2.0, (double)n);
        for (int i = 0; i < tamanho; i++)
        {
            contSaturado.push_back(0);
        }
    }

    void atualiza(unsigned int i, bool tomado)
    {
        // cout << "update PHT num " << i << endl;

        if (tomado)
        {
            if (contSaturado[i] < 3)
                contSaturado[i]++;
        }
        else
        {
            if (contSaturado[i] > 0)
                contSaturado[i]--;
        }
        // cout << "pos update PHT" << endl;
    }

    // Retorna o bit de previsão da entrada n do PHT
    bool previsao(unsigned int n)
    {
        if (contSaturado[n] < 2)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
};

// Retorna uma lista com 2^n BHRs de tamanho k
vector<BHR> criaBHT(int n, int k){
    vector<BHR> BHT;
    
    int qtdBHR = (int)(pow(2.0, (double)n));
    for (int i = 0; i<qtdBHR; i++){
        BHT.push_back(BHR(k));
    }

    return BHT;
}

// Retorna uma lista com 2^n PHTs de tamanho 2^k
vector<PHT> criaListaPHT(int n, int k){
    vector<PHT> listaPHT;
    
    int qtdPHT = (int)(pow(2.0, (double)n));
    for (int i = 0; i<qtdPHT; i++){
        listaPHT.push_back(PHT(k));
    }

    return listaPHT;
}


// Retorna o número em base 10 equivalente aos n bits mais significativos do endereço
uintptr_t msb(uintptr_t endereco, unsigned int n)
{
    unsigned int tamanho = sizeof(endereco);
    if (n < tamanho * 8)
    {
        uintptr_t retorno = endereco >> (tamanho * 8 - n);
        return retorno;
    }
    else
    {
        return endereco;
    }
}

// Retorna o número em base 10 equivalente aos n bits menos significativos do endereço
uintptr_t lsb(uintptr_t endereco, unsigned int n)
{
    if (n < sizeof(endereco) * 8)
    {
        uintptr_t mascara = (uintptr_t)(pow(2.0, (double)n) - 1);
        uintptr_t retorno = endereco & mascara;
        return retorno;
    }
    else
    {
        return endereco;
    }
}

#endif /* _BP_HELPER_H_ */
