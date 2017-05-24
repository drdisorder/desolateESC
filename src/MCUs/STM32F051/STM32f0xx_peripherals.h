
void EnablePheriphClocks(void);
void TimeTimerInit(void);
uint16_t micros16(void);
void delay16(uint16_t delay);
void PWMTimerInit(void);
void setPWMduty(uint32_t duty);
void ComparatorInit(void);
void setComparatorInput(uint8_t phase);
void setComparatorEdge(uint8_t edge);
uint8_t getComparatorOut(void);
void setComparatorInterruptStatus(uint8_t Enable);
uint16_t getTimeSource(void);