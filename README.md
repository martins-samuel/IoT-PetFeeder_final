
# 🐾 Pet Feeder Automático IoT

## 📄 Descrição
Este projeto é um alimentador automático para pets controlado por **IoT**, desenvolvido com **NodeMCU v1**.  
O sistema utiliza um **motor de passo 28BYJ-48** com **driver ULN2003** para controlar a liberação de ração, enquanto o **sensor ultrassônico HC-SR04** monitora o nível de ração no compartimento.  
A alimentação é feita por um **power bank**, permitindo mobilidade e uso contínuo.

O controle do pet feeder é feito através de uma **página web hospedada no Firebase**, onde o usuário pode definir horários, liberar porções de ração manualmente e acompanhar o status do sistema em tempo real.

---

## ⚙ Hardware Utilizado
- **NodeMCU v1 (ESP8266)**: Controlador principal do sistema.
- **Motor de Passo 28BYJ-48**: Responsável pelo mecanismo de liberação da ração.
- **Driver ULN2003**: Interface entre o NodeMCU e o motor de passo.
- **Sensor Ultrassônico HC-SR04**: Mede a quantidade de ração restante no compartimento.
- **Power Bank**: Fonte de alimentação portátil para o sistema.

---

## 🔧 Funcionamento
1. O **motor de passo** gira para liberar uma porção controlada de ração para o pet.
2. O **sensor ultrassônico** verifica o nível de ração para alertar quando estiver próximo do fim.
3. O **NodeMCU** se comunica com a **página web hospedada no Firebase** para receber comandos e enviar dados em tempo real, permitindo o controle remoto e monitoramento pelo usuário.

---

## 🌐 Estrutura do Site Web
A interface web do **Pet Feeder Automático**, que é hospedada no Firebase, é dividida em três arquivos principais que trabalham em conjunto para proporcionar a experiência do usuário:

- **index.html**: Arquivo principal que define a estrutura da página, incluindo botões, campos de agendamento e exibição do nível de ração e histórico.
- **script.js**: Arquivo JavaScript responsável pela lógica de interação com o Firebase, enviando comandos de alimentação, salvando horários e atualizando informações em tempo real.
- **style.css**: Define a estilização da página, garantindo uma aparência agradável e responsiva para diferentes dispositivos.

![image](https://github.com/user-attachments/assets/645f7f8f-1422-4a1b-978d-5a3d80935a65)

---

## 🖨 Estrutura e Impressão 3D
A estrutura física do projeto, incluindo carcaça e suporte para os componentes, foi baseada no design disponível no **Hackster**:  
[Pet Feeder com ESP8266 - Projeto no Hackster](https://www.hackster.io/sokolsok/pet-feeder-with-esp8266-6ceef2)  
Lá você encontra os arquivos para impressão 3D e detalhes para montagem da estrutura.

---

## 🚀 Como usar
1. Monte o circuito conectando o **motor de passo ao driver ULN2003**, e este ao **NodeMCU**.
2. Instale o **sensor ultrassônico** sobre o compartimento de ração para medir o nível.
3. Alimente o sistema com o **power bank**.
4. Faça o **upload do código** para o **NodeMCU** pela **IDE Arduino**.
5. Acesse a **página web hospedada no Firebase** para configurar horários, controlar a liberação da ração e monitorar o status do pet feeder.
6. Aproveite o sistema automatizado para alimentar seu pet de forma prática e remota! 🐶🐱

---

## 🔌 Esquema de ligação
*(Adicionar imagem ou link com o diagrama das conexões)*

---

## 💡 Inspiração
Este projeto foi inspirado pelo vídeo **"Automatic Pet Feeder using NodeMCU and Firebase"**, que serviu como base para a criação da solução de alimentação automatizada com controle via web.




