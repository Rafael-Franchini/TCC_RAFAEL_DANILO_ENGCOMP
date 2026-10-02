# 🔥 Sistema Embarcado de Detecção contra Incêndio com Integração a Painel Elétrico CCM/CLP

> Detecção precoce de incêndio em painéis elétricos industriais com Arduino Nano, sensores de gás, temperatura e chama, e integração a CLP.

![Status](https://img.shields.io/badge/status-concluído-success)
![Tipo](https://img.shields.io/badge/tipo-TCC-blue)
![Plataforma](https://img.shields.io/badge/plataforma-Arduino%20Nano-00979D)
![Linguagem](https://img.shields.io/badge/linguagem-C%2FC%2B%2B-informational)
![Ano](https://img.shields.io/badge/ano-2026-lightgrey)

---

## 📌 Sobre o trabalho

Este repositório reúne o Trabalho de Conclusão de Curso (TCC) desenvolvido no **Centro Universitário da Fundação Hermínio Ometto (FHO)**, curso de **Engenharia de Computação**.

### Resumo

O aumento da ocorrência de incêndios em ambientes industriais e residenciais evidencia a necessidade de sistemas eficientes para detecção em estágio inicial de situações de risco. Este trabalho apresenta o desenvolvimento de um sistema embarcado de detecção de incêndios baseado no microcontrolador **Arduino Nano**, integrado a sensores de gás (**MQ-2**), temperatura (**BMP280**) e chama infravermelha.

O sistema monitora continuamente o ambiente. Quando uma condição de risco é detectada, um módulo relé é acionado automaticamente, permitindo o disparo de alarmes e a integração com sistemas de automação industrial por meio de um **Controlador Lógico Programável (CLP)**. A solução foi projetada para painéis elétricos do tipo **Centro de Controle de Motores (CCM)**, seguindo diretrizes de segurança e normas técnicas.

Nos testes experimentais em ambiente controlado, o sistema respondeu em **menos de 2 segundos**, com boa confiabilidade, estabilidade e baixo custo.

**Palavras-chave:** Detecção de incêndio · Sistemas embarcados · Automação industrial

---

## 👥 Autores

| Papel | Nome | Instituição |
|-------|------|-------------|
| Autor | Danilo Henrique Muller | FHO |
| Autor | Rafael Alexandre Lima Franchini | FHO |
| Orientador | Maurício Acconcia Dias | FHO |

- **Data de defesa:** 18/06/2026

---

## 📄 Documento

| Item | Link |
|------|------|
| 📕 Artigo/TCC completo (PDF) | [Baixar documento](./docs/Sistema_Embarcado_de_Deteccao_contra_Incendio.pdf) |
| 🎞️ Apresentação (slides) | [Ver slides](./docs/apresentacao.pdf) |
| 🎥 Vídeo do protótipo | [Assistir](#) |

---

## 🎯 Problema e objetivos

**Problema de pesquisa:** como desenvolver um sistema de detecção de incêndio de baixo custo, com resposta rápida e integração a sistemas de automação industrial?

**Objetivo geral:** desenvolver um sistema embarcado de detecção de incêndios com Arduino Nano, integrado a sensores ambientais e capaz de atuar automaticamente por meio de relés, com integração a um painel CCM e envio de sinais a um CLP.

**Objetivos específicos**
- Desenvolver o circuito eletrônico do sistema
- Implementar a lógica de detecção e acionamento automático
- Integrar o sistema ao CLP
- Avaliar o desempenho da solução por meio de testes experimentais

**Motivação:** um caso real de princípio de incêndio em painel elétrico industrial, causado pela troca de um inversor de frequência sem adequação da fiação existente, o que gerou aquecimento excessivo dos condutores.

---

## 🧩 Arquitetura do sistema

```
Sensores (MQ-2 · BMP280 · Chama IR)
              │
              ▼
      Arduino Nano (ATmega328, 16 MHz)
              │
              ▼
      Módulo Relé 2 canais
        │               │
        ▼               ▼
   CLP (CCM)      Alarme local
 (intertravamento) (buzzer + LED 24 V)
```

<img width="957" height="600" alt="image" src="https://github.com/user-attachments/assets/111aa453-e69e-4f2e-928d-2415548ffd9c" />


### Componentes

| Componente | Função | Interface |
|------------|--------|-----------|
| Arduino Nano (ATmega328) | Leitura dos sensores, decisão e acionamento | — |
| Sensor de gás **MQ-2** | Gases inflamáveis (GLP, metano, butano) | Entrada analógica |
| Sensor de temperatura **BMP280** | Elevação térmica anormal | I²C |
| Sensor de **chama infravermelho** | Radiação emitida por chamas | Entrada digital |
| Módulo relé 2 canais | Isolamento entre controle e potência | Saída digital |
| Buzzer com LED (24 V DC) | Sinalização sonora e visual local | Relé 2 |
| Fonte 24 V DC + conversor 5 V | Alimentação padrão industrial | — |

### Pinagem e parâmetros (Arduino Nano)

| Pino | Ligação |
|------|---------|
| `A0` | Saída analógica do MQ-2 |
| `D2` | Sensor de chama infravermelho (ativo em nível baixo) |
| `D4` | Relé 2 → buzzer com LED (alarme local) |
| `D5` | Relé 1 → sinal para o CLP |
| `A4` / `A5` | BMP280 via I²C (SDA/SCL), endereço `0x76` |

| Parâmetro | Valor |
|-----------|-------|
| Limiar de gás (leitura analógica) | `400` |
| Limiar de temperatura | `45,0 °C` |
| Mínimo de sensores em alerta | `2` |
| Intervalo de amostragem | `500 ms` |

Os módulos relé são ativados em **nível baixo (LOW)**. Os limiares devem ser ajustados na calibração, conforme o ambiente.

### Protótipo

<img width="946" height="1066" alt="image" src="https://github.com/user-attachments/assets/c393e69c-8876-409e-97e9-78900b5f9ddd" />


---

## 🧠 Lógica de controle

Para reduzir falsos positivos, o sistema usa **redundância parcial**: o acionamento só ocorre quando **pelo menos 2 dos 3 sensores** detectam condição de risco simultaneamente.

```
Acionamento = (Gás E Temperatura) OU (Gás E Chama) OU (Temperatura E Chama)
```

Quando a condição é satisfeita, os dois canais do relé são acionados:

1. **Relé 1** → envia sinal ao CLP do painel CCM (desligamento de cargas e intertravamentos)
2. **Relé 2** → aciona o alarme sonoro e luminoso local

<img width="942" height="1348" alt="image" src="https://github.com/user-attachments/assets/e0d45a09-14fb-4fec-be17-9c69e4c82290" />


---

## 🧪 Metodologia dos testes

Pesquisa aplicada, de abordagem quantitativa e natureza experimental. Os testes foram feitos em ambiente controlado:

- **Gás:** butano (isqueiro, sem chama) próximo ao MQ-2
- **Chama:** fonte de fogo controlada
- **Temperatura:** fonte de calor próxima ao BMP280

Parâmetros avaliados: tempo de resposta, estabilidade das leituras, confiabilidade da detecção e correto acionamento dos relés e saídas. A integração com o CLP foi validada pela verificação do recebimento do sinal digital do relé.

---

## 📊 Resultados

| Condição simulada | Sensores acionados | Detecção | Tempo de resposta |
|-------------------|--------------------|:--------:|:-----------------:|
| Gás (butano) | MQ-2 | Não* | — |
| Chama isolada | Chama | Não* | — |
| Aumento de temperatura | BMP280 | Não* | — |
| Gás + temperatura | MQ-2 + BMP280 | ✅ Sim | ~1,8 s |
| Gás + chama | MQ-2 + Chama | ✅ Sim | ~1,5 s |
| Chama + temperatura | Chama + BMP280 | ✅ Sim | ~1,2 s |

\* Sem acionamento por causa da lógica de validação (mínimo de dois sensores), o que confirma o funcionamento esperado.

**Principais achados**
- Tempo médio de resposta global **inferior a 2 segundos**
- A lógica de redundância parcial reduziu significativamente os falsos positivos
- O MQ-2 teve melhor desempenho a distâncias **menores que 30 cm**
- O sensor de chama é o mais rápido, e o BMP280 é mais indicado para aquecimento progressivo (sobrecarga elétrica)

### ⚠️ Limitações observadas

- **MQ-2:** sensível a vários tipos de gás, exige calibração conforme o ambiente; a ventilação forçada do painel dispersa o gás e aumenta o tempo de detecção
- **Sensor de chama:** a luminária interna do painel (acionada por fim de curso) causou falso acionamento, corrigido com ajuste de sensibilidade
- **BMP280:** resposta gradual; os limites foram ajustados acima da faixa normal de operação do painel (cerca de 40 °C a 50 °C)
- Ausência de comunicação remota

---

## 🚀 Trabalhos futuros

- Comunicação remota com tecnologias **IoT**
- Sensores industriais de maior precisão
- Algoritmos mais avançados de análise dos dados

---

## 📋 Normas consideradas

- **ABNT NBR 17240** – Sistemas de detecção e alarme de incêndio
- **ABNT NBR 5410** – Instalações elétricas de baixa tensão
- **NR-10** – Segurança em instalações e serviços em eletricidade

---

## 📂 Estrutura do repositório

```
.
├── code arduino/
│   └── sensores/        # Código do Arduino Nano (leitura dos sensores e lógica de acionamento)
├── docs/
│   ├── Sistema_Embarcado_de_Deteccao_contra_Incendio.pdf
│   └── apresentacao.pdf
├── assets/              # Figuras do trabalho
│   ├── figura1-arquitetura.png
│   ├── figura2-prototipo.jpg
│   └── figura3-fluxograma.jpg
├── LICENSE              # MIT
└── README.md
```

---

## ▶️ Como reproduzir

1. Monte o circuito conforme a arquitetura acima (alimentação 24 V DC com conversor para 5 V)
2. Abra o código da pasta [`code arduino/sensores`](./code%20arduino/sensores) na **Arduino IDE**
3. Instale as bibliotecas **Adafruit BMP280** e **Adafruit Unified Sensor** (a `Wire` já vem com a IDE)
4. Ajuste os limiares dos sensores ao seu ambiente (principalmente temperatura e sensibilidade do sensor de chama)
5. Carregue o código no Arduino Nano

> ⚠️ Este é um protótipo acadêmico. Para uso em instalações reais, valide o projeto conforme as normas aplicáveis e com profissional habilitado.

---

## 📚 Como citar

```
MULLER, Danilo Henrique; FRANCHINI, Rafael Alexandre Lima; DIAS, Maurício Acconcia.
Sistema embarcado de detecção contra incêndio com integração a painel elétrico CCM CLP.
2026. Trabalho de Conclusão de Curso – Centro Universitário da Fundação Hermínio
Ometto (FHO), Araras, 2026.
```

---

## 📜 Licença

Este projeto está licenciado sob a **Licença MIT**. Consulte o arquivo [LICENSE](./LICENSE).

---

## 📬 Contato

- 🐙 GitHub: [@Rafael-Franchini](https://github.com/Rafael-Franchini)
- 💼 LinkedIn: [Rafael Franchini](https://www.linkedin.com/in/rafael-franchini-37b0a21a4/)
