


#include <avr/io.h>
#include <avr/interrupt.h>


#define BUT_PIN 0x10
#define LED_PIN 0x01
#define FIVE_MS 156

#define MAGIC_VALUE 250
#define MAGIC_VALUE_DELAY 750
#define INCREMENT 5

int state = 1;
int state_led = 0;
int count_button_push = 0;
int count_button_delay = 0;

// проверка нажатия кнопки
inline int is_button_push (void) {
	return !(PINB & BUT_PIN);
}

inline void led_on(void) {
	state_led = 1;
	PORTB &= 0xfe;
}

inline void led_off(void) {
	state_led = 0;
	PORTB |= 0x01;
}
// смена состояния светодиода, используется переменная state
void reverse_led (void) {
	if (state_led == 0) {
		led_on();
	} else {
		led_off();
	}

}

void verse_led (void) {
	if (state_led == 0) {
		led_off();
	} else {
		led_on();
	}
}

ISR (TIMER1_COMPA_vect) {

	// кнопка нажата
    if(is_button_push()/*== TRUE*/) {
	
		// если мы не мигаем
		if(state == 0) count_button_push += INCREMENT;
		
		// если мы в режиме мигания
		if(state == 1) count_button_delay += INCREMENT;		
		
		// если мы мигаем и превысили интервал - инвертировать светодиод
		if((count_button_delay >= MAGIC_VALUE_DELAY) && (state == 1)) {
			reverse_led();
			count_button_delay = 0;
			
		}
		
		// если мы не врежиме мигания
		if((count_button_push >= MAGIC_VALUE) && (state == 0)) {
			state = 1;
			count_button_delay = 0;
			verse_led();
		}
	}
	
	// кнопка отпущена
	else if(state == 1) {
	         
			state = 0;
			count_button_push = 0;
			led_off();
		}
}

int main ()
{
    //pin 0 - led act GND, pin 4 - button act GND
    DDRB = 0x01;
    PORTB = 0x10;
	// разрешаем прерывания
	SREG = 0x80;
	
    //CTC, presc 256
    TCCR1B = _BV(WGM12) | _BV(CS12);
    TIMSK  = _BV(OCIE1A);
    OCR1A  = FIVE_MS;
	
    while (1) {
    } 

    return 0;
}
