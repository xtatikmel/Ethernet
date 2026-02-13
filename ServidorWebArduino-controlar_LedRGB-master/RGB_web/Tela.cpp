#include "Tela.h"


void Tela::Escopo(){
  conexao.println("<!DOCTYPE html>");
  conexao.println("<html lang=\"pt-br\">");
  conexao.println("<head>");
  conexao.println("<title>Projeto Web Controle</title>");
  conexao.println("</head>");
}

void Tela::Tela_login(){
          Escopo();
          conexao.println("<body style=\"background-color: #B5B5B5; margin: auto; text-align: center \">");
          conexao.println("<br>");
          conexao.println("<form style=\"position: relative; background-color: #828282; text-align: center;\">");
          conexao.println("<br>");
          conexao.println("<button type=\"submit\" style=\"width:700px;height:200px; padding:25px; margin:25px; font-size: 40px; color: #FF0000;\"  formaction=\"btVermelho\"> <b> VERMELHO </b> </button>");//Button HTML5
          conexao.println("<br>");
          conexao.println("<button type=\"submit\" style=\"width:700px;height:200px; padding:25px; margin:25px; font-size: 40px; color: #00FF00;\" formaction=\"btVerde\"> <b> VERDE </b> </button>");
          conexao.println("<br>");
          conexao.println("<button type=\"submit\" style=\"width:700px;height:200px; padding:25px; margin:25px; font-size: 40px; color: #0000FF;\" formaction=\"btAzul\"> <b> AZUL </b> </button>");
          conexao.println("<br>");
          conexao.println("<button type=\"submit\" style=\"width:700px;height:200px; padding:25px; margin:25px; font-size: 40px;\" formaction=\"btDesligar\"> <b> DESLIGAR </b> </button>");
          conexao.println("<br>");
          conexao.println("<button type=\"submit\" style=\"width:350px;height:50px; padding:25px; margin:25px; font-size: 20px;\" formaction=\"btSair\"> <b> SAIR </b> </button>");
          conexao.println("</form>");
          conexao.println("<br>");
          conexao.println("</body>");
          conexao.println("</html>");
}
