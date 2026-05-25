// ======================================================
// PROIECT UART / USART - ATmega328P
// ======================================================
// Obiective:
// 1. Configurare UART hardware
// 2. Transmitere date UART
// 3. Recepție date UART
// 4. Protocol text prin serială
// Componente:
// - 2 LED-uri
// - 1 Buton
// - UART serial communication
// ======================================================
#include "Arduino.h"
#include <Uart.h>
// ======================================================
// OBIECT UART
// ======================================================
// obiect folosit pentru comunicația serială UART
Uart uart;
// ======================================================
// ENUMERĂRI
// ======================================================
// stările posibile ale butonului
enum ButtonState {
    PRESSED, // buton apăsat
    NOT_PRESSED // buton neapăsat
};
// stările posibile ale LED-urilor
enum LedState {
    ON, // LED aprins
    OFF // LED stins
};
// ======================================================
// VARIABILE GLOBALE
// ======================================================
// pinul butonului
int BUTTON_PIN = 0;
// ======================================================
// PIND = Input Register pentru PORTD
// citește starea logică a pinilor PD0 ... PD7
// (1 << 2) => selectează bitul corespunzător lui PD2
// dacă rezultatul este diferit de 0:
// pinul PD2 este HIGH
// ======================================================
// citește starea pinului PD2
unsigned char btn = PIND & (1 << 2);
// ======================================================
// FUNCȚIE CITIRE BUTON
// ======================================================
ButtonState inline readBtnState() {
    // ==================================================
    // PIND = registru de citire pentru PORTD
    //
    // citește valoarea logică de pe PD2
    // ==================================================
    btn = PIND & (1 << 2);
    // dacă PD2 este HIGH
    if (btn != 0) { 
        // buton apasat
        return ButtonState::PRESSED;
        uart.writeString("Buton apasat lol\n");
    } else {   
        // buton neapăsat
        return ButtonState::NOT_PRESSED;
    } 
}
// ======================================================
// CONTROL LED0
// ======================================================
void inline setLed0State(LedState state) {
    // ==================================================
    // LED0 este conectat pe PD3
    // ==================================================
    if (state == LedState::ON) {
        // ==============================================
        // PORTD = registru de ieșire pentru PORTD
        // |=  => setează bitul la 1
        // (1 << 3) => selectează bitul PD3
        // rezultat:
        // PD3 = HIGH - LED aprins
        // ==============================================
        PORTD |= (1 << 3); // aprins
    } else {
        // ==============================================
        // &= ~(...) => pune bitul pe 0
        // rezultat:
        // PD3 = LOW
        // LED stins
        // ==============================================
        PORTD &= ~(1 << 3); // stins
    }
}
// ======================================================
// CONTROL LED1
// ======================================================
void inline setLed1State(LedState state) {
   // LED1 conectat pe PD4
    if (state == LedState::ON) {
        PORTD |= (1 << 4); // aprins
    } else {
        PORTD &= ~(1 << 4); // stins
    }
}
// ======================================================
// SETUP
// ======================================================
void setup() {
    // ==================================================
    // DDRD = Data Direction Register D
    // stabilește direcția pinilor:
    // 0 = INPUT
    // 1 = OUTPUT
    // ==================================================
    DDRD &= ~(1 << 2);  // PD2 is Input (Button)
    DDRD |= (1 << 3);  // PD3 is Output (LED0)
    DDRD |= (1 << 4);  // PD4 is Output (LED1)
    // ==================================================
    // PORTD controlează nivelul logic al pinilor
    // 0 = LOW
    // 1 = HIGH
    // ==================================================
    PORTD &= ~(1 << 3); // stinge LED0
    PORTD &= ~(1 << 4); // stinge LED1
    // ==================================================
    // inițializează UART
    // ==================================================
    uart.init();
    // trimite caracterul 'a'
    uart.writeByte('a');
    // ==================================================
    // DDRB = Data Direction Register B
    // configurează direcția pinilor PB0 ... PB7
    // ==================================================
    // configurează PB1 (OC1A) ca OUTPUT
    DDRB |= (1 << 1); // OC1A pin as output

}
// ======================================================
// FUNCȚIE RĂSPUNS UART
// ======================================================
void responseBack(const char* userMsg) {
    // trimite mesaj de răspuns pe serială
    uart.writeString("You said: ");
    // trimite mesajul utilizatorului
    uart.writeString(userMsg);
    // newline
    uart.writeString("\n");
}

// ======================================================
// BUFFER UART
// ======================================================
// buffer pentru caracterele primite
char buffer[1024];
// index curent în buffer
int index = 0;
// ======================================================
// LOOP PRINCIPAL
// ======================================================
void loop() {  
    // ==================================================
    // verifică dacă există date disponibile pe UART
    // ==================================================  
    if (uart.available()) {
        // citește un caracter primit
        char c = uart.readByte();
        // dacă utilizatorul a trimis ENTER
        if (c == '\n') {
            // termină șirul cu caracter NULL
            buffer[index] = '\0';
            // ==========================================
            // COMANDA: ledon
            // ==========================================
            if (strcmp(buffer, "ledon") == 0) {
                // aprinde LED0
                setLed0State(LedState::OFF);
            } else if (strcmp(buffer, "ledoff") == 0) {
                // stinge LED0
                setLed0State(LedState::ON);
            // ==========================================
            // COMANDA: oscledon
            // activează PWM pe OC1A
            // ==========================================
            } else if (strcmp(buffer, "oscledon") == 0) {
                // ======================================================
                // CONFIGURARE TIMER1 PENTRU PWM (Pulse Width Modulation)
                // ======================================================
                // ======================================================
                // TCCR1A = Timer/Counter1 Control Register A
                // configurează:
                // - modul PWM
                // - comportamentul ieșirii OC1A
                // COM1A1 = 1
                // COM1A0 = 0
                // => mod non-inverting PWM
                // WGM11 = 1 (Waveform Generation Mode)
                // WGM10 = 1 
                // => parte din configurarea modului Fast PWM
                // ======================================================
                TCCR1A = (0b10<<0) | (0b11<<6);
                // ======================================================
                // TCCR1B = Timer/Counter1 Control Register B
                // WGM13 = 1
                // WGM12 = 1
                // împreună cu:
                // WGM11 = 1
                // WGM10 = 1
                // rezultă:
                // WGM13 WGM12 WGM11 WGM10
                //    1     1     1     1
                // => MODE 15 din datasheet
                // => Fast PWM
                // CS12 = 1
                // CS11 = 0
                // CS10 = 1
                // => prescaler = 1024
                // timerul rulează la:
                // 16MHz / 1024
                // ======================================================
                TCCR1B = (0b11<<3) | (0b101<<0);
                // ======================================================
                // FORMULA PWM
                // fPWM = fCPU / (N * (1 + TOP))
                // unde:
                // fCPU = 16MHz
                // N = prescaler (prescaler-ul timerului)
                // TOP = ICR1
                // Dorim aproximativ:
                // fPWM ≈ 1.42 Hz
                // Calcul:
                // TOP = (fCPU / (N * fPWM)) - 1
                // TOP = (16000000 / (1024 * 1.42)) - 1
                // TOP ≈ 10937
                // ICR1 stabilește valoarea TOP
                // și perioada semnalului PWM
                // ======================================================
                ICR1 = 10937;
                // ======================================================
                // DUTY CYCLE PWM
                // Formula:
                // DutyCycle = OCR1A / TOP * 100
                // Dorim aproximativ:
                // 71% duty cycle
                // Calcul:
                // OCR1A = 0.71 * 10937
                // OCR1A ≈ 7812
                // OCR1A stabilește cât timp semnalul
                // rămâne HIGH într-o perioadă PWM
                // ======================================================
                OCR1A = 7812;
                // ==========================================
                // COMANDA: oscledoff
                // oprește PWM
                // ==========================================
            } else if (strcmp(buffer, "oscledoff") == 0) {
                // oprește Timer1
                TCCR1A = 0;
                TCCR1B = 0;
                // resetează registrele PWM
                ICR1 = 0;
                OCR1A = 0;
                // pune PB1 pe LOW
                PORTB &= ~(1 << 1);
            }
            // trimite răspuns către PC
            responseBack(buffer);
            // resetează indexul bufferului
            index = 0;
        } else if (c != '\r') {
            // ==========================================
            // salvează caracterul în buffer
            // ==========================================
            buffer[index++] = c;
        }
    }
}