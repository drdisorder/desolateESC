
void EnablePheriphClocks(void);
void TimeTimerInit(void);
uint16_t micros16(void);
void delay16(uint16_t delay);
void SignalInputInit(void);
void PWMTimerInit(void);
void setPWMcompares(uint32_t compare);
void ComparatorInit(void);
void setComparatorInput(uint8_t phase);
void setComparatorEdge(uint8_t edge);
uint8_t getComparatorOut(void);
void setComparatorInterruptStatus(uint8_t Enable);
uint16_t getTimeSource(void);
void TLMUARTInit(void);
void TLMUARTsend(uint8_t *buf, uint8_t length);
void ADCInit(void);
void getADCvalues(int16_t *ADCdatas);