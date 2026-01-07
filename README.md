# Node-Bee

**Node-Bee** é um dispositivo educacional baseado em ESP32 que identifica redes Wi-Fi abertas próximas e permite conectar-se a elas para fins de **observação passiva e aprendizado de redes e segurança**.  
O projeto é **open source** e tem como objetivo ensinar sobre redes, protocolos Wi-Fi, microcontroladores e lógica de sistemas embarcados.

> ⚠️ Disclaimer: Node-Bee é estritamente **educacional**. Não possui funcionalidades destrutivas ou invasivas. Qualquer uso fora do escopo educacional e ético **não está vinculado ao projeto original**.

---

## Objetivos do Projeto

- Identificar redes Wi-Fi abertas disponíveis no ambiente.  
- Navegar entre redes com botões táteis.  
- Conectar-se a redes abertas de forma legítima.  
- Registrar informações de redes e status de conexão em microSD para análise educacional.  

---

## Componentes Necessários

- **1x ESP32 NodeMCU** – microcontrolador principal  
- **2x Botões táteis** – navegação e seleção de redes  
- **1x Tela OLED 1.3”** – exibe SSIDs, status e informações do dispositivo  
- **1x Bateria 5V** – para tornar o dispositivo portátil  

---

## Arquitetura do Projeto

### Hardware

- ESP32 conecta aos botões, tela OLED e microSD.  
- Botões:  
  - **Botão A** → avançar para próxima rede  
  - **Botão B** → selecionar/conectar à rede  
- Tela OLED exibe informações do estado atual e redes disponíveis.  
- MicroSD registra logs de redes e status de conexão.  
- Bateria 5V alimenta o dispositivo de forma móvel.  

### Software

O firmware do Node-Bee é dividido em módulos:

- `wifi_scan` → escaneamento e filtragem de redes abertas  
- `fsm_control` → máquina de estados e botões  
- `sd_logger` → gravação de logs no microSD  
- `display` → atualização da tela OLED  
- `main` → loop principal, inicialização e integração dos módulos  

---

## Máquina de Estados (FSM)

1. **Idle** → espera inicial  
2. **Scan** → detecta redes abertas próximas  
3. **Navegação** → Botão A percorre a lista de redes  
4. **Conexão** → Botão B conecta à rede selecionada  
5. **Logging** → registra informações no microSD  

> A FSM garante que cada ação seja controlada e previsível, facilitando testes e aprendizado.

---

## Instalação e Testes

1. Conecte o ESP32 ao computador via USB.  
2. Instale as bibliotecas necessárias:  
   - `WiFi.h`  
   - `SPI.h` e `SD.h`  
   - `Adafruit_SSD1306` (para OLED)  
3. Compile e envie o código.  
4. Teste cada módulo individualmente:  
   - Scan Wi-Fi  
   - Botões  
   - Tela OLED  
   - Logging no microSD  
5. Após testes bem-sucedidos, o Node-Bee pode ser usado com bateria 5V para mobilidade.  

---

## Ética e Uso Responsável

- Node-Bee é **para fins educacionais apenas**.  
- Não deve ser usado para invadir redes ou acessar dispositivos sem permissão.  
- Qualquer modificação fora do escopo ético **não é responsabilidade dos desenvolvedores**.  

---

## Futuras Expansões

- Adicionar LEDs ou buzzer para feedback visual/auditivo.  
- Armazenar histórico detalhado de redes com RSSI.  
- Otimizar consumo de energia para maior autonomia móvel.  

---

## Licença

Este projeto é **open source**, licenciado sob MIT License.  
O código pode ser estudado, modificado e compartilhado, **desde que o uso continue educacional e ético**.
