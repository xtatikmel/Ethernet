# Servidor Web no Arduino para controlar Led RGB

<ul>
		Projeto base de um servidor web no arduino usando o módulo Ethernet Shield W5100. Na arquiteura do projeto é possivel criar paginas com facilidade para controle de diversos componentes. <br>
	 Para isso basta criar a pagina (em html) como um método no arquivo "Tela.cpp" declarar o método no arquivo "Tela.h", após, é preciso instância-lo na classe principal (.ino) e chamar os métodos(telas) que foram criadas no arquivo Tela.cpp no loop da classe principal.
	</ul>

#### Materiais utilizados
<ul>
	Arduino UNO +  Ethernet Shield W5100 + Led RGB
	</ul>

#### Configuração de rede
<ul>
	Para tal funcionamento será necessário a configuração da rede, tanto no arduino como no roteador. No Arduino o servidor roda na porta 80 e ip de máscara /24. 
	No roteador é preciso direcionar a porta 80 para o ip que foi setado no servidor.
</ul>

#### Conclusão
<ul>
	É possivel utilizar este projeto para controle de diversos componentes através de uma intranet, no meu caso usei o led RGB como componente. Tenho uma organização aqui no GitHub que possui classes prontas e testadas para controle de diversos componentes.  <br>                   
	Link para acesso - https://github.com/WendelLorenzi/arduino-components
</ul>
