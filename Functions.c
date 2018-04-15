/*
 *  ABOUT
 *
 * 	Date:		Abril del 2017
 *
 * 	Authors:	MARISCAL ARROYO JUAN PABLO
 * 				MORENO ALONZO LUIS FERNANDO
 * 				ZUÑIGA CARRILLO MARIO EUGENIO
 *
 * 	Description: Práctica #2
 *
 */

/*******************************************************************************
 * LIBRARIES
 ******************************************************************************/
#include "Definitions.h"

/*******************************************************************************
 * GLOBAL HANDLER VARIABLES
 ******************************************************************************/
SemaphoreHandle_t xBotton1_SemaphoreHandler = NULL;
SemaphoreHandle_t xBotton2_SemaphoreHandler = NULL;
SemaphoreHandle_t xBotton3_SemaphoreHandler = NULL;
SemaphoreHandle_t xBotton4_SemaphoreHandler = NULL;

SemaphoreHandle_t xMutexHandler;
size_t n;

/*******************************************************************************
 * GLOBAL STRING VARIABLES
 ******************************************************************************/

uint8_t Welcome_string[] = "- - - - - - P R A C T I C A 2 - - - - - -\r\n\r\n";
uint8_t Option0_string[] = "0)   Chat\r\n";
uint8_t Option1_string[] = "1)   Leer Memoria I2C\r\n";
uint8_t Option2_string[] = "2)   Escribir Memoria I2C\r\n";
uint8_t Option3_string[] = "3)   Establecer hora\r\n";
uint8_t Option4_string[] = "4)   Leer hora\r\n";
uint8_t Option5_string[] = "5)   Formato de hora\r\n";
uint8_t Option6_string[] = "6)   Establecer fecha\r\n";
uint8_t Option7_string[] = "7)   Leer fecha\r\n";
uint8_t Option8_string[] = "8)   Leer Acelerometro\r\n";
uint8_t Option9_string[] = "9)   Leer Termometro\r\n\r\n";
uint8_t Write_string[] = "Ingrese opcion:	";

/* ERROR MESSAGE*/
uint8_t menssage_error_1[] = "\r\n--------> ALERTA <-------- \r\n\r\n";
uint8_t menssage_error_2[] = "Este recurso esta en uso.\r\n";

uint8_t ESC_string[] = "Presione [<] para regresar.";
uint8_t ESC_string_2[] = "\r\n\r\nPresione [<] para salir...\r\n\r\n";
uint8_t BTN_string[] = "\r\nPresione [BOTON 1] para salir...\r\n";

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/

/*!
 * @brief i2c_release_bus_delay().
 *
 * Delay para que el bus sea liberado.
 *
 */
void i2c_release_bus_delay(void) {

    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++) {
        __NOP();
    }
}

/*!
 * @brief BOARD_I2C_ReleaseBus().
 *
 * Configura e inicializa el bus de la I2C.
 *
 */
void BOARD_I2C_ReleaseBus(void) {

    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config = {0};

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux = kPORT_MuxAsGpio;

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA low */
    for (i = 0; i < 9; i++) {

        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}

/*!
 * @brief I2C_WriteReg().
 *
 * Escribe en el dispositivo I2C correspondiente, en el registro correspondiente un valor determinado.
 *
 */

bool I2C_WriteReg(I2C_Type *base, uint8_t device_addr, uint16_t reg_addr, uint8_t* value) {
		i2c_master_transfer_t masterXfer;
		memset(&masterXfer, 0, sizeof(masterXfer));

		masterXfer.slaveAddress = device_addr;
		masterXfer.direction = kI2C_Write;
		masterXfer.data = value;
		masterXfer.dataSize = strlen(value);
		masterXfer.flags = kI2C_TransferDefaultFlag;
		masterXfer.subaddress = reg_addr;

		if(MEMORY_ADDRESS == device_addr){
			masterXfer.subaddressSize = 2;
		}else if((RTC_ADDRESS == device_addr)||(ACCEL_ADDRESS == device_addr)){
			masterXfer.subaddress = (uint8_t) reg_addr;
			masterXfer.subaddressSize = 1;
		}
		I2C_MasterTransferNonBlocking(BOARD_I2C0_BASEADDR, get_I2CMasterHandle(), &masterXfer);
		vTaskDelay(100);
		/*  wait for transfer completed. */
		while ((!get_nakFlag()) && (!get_CompletionFlag())) {
		}

		set_nakFlag(false);

		if (get_CompletionFlag() == true) {
			set_CompletionFlag(false);
			return true;
		} else {
			return false;

		}
}

/*!
 * @brief I2C_WriteReg_Hex().
 *
 * Escribe en el dispositivo I2C correspondiente, en el registro correspondiente un valor determinado
 * para los valores en hexadecimal.
 *
 *
 */
bool I2C_WriteReg_Hex(I2C_Type *base, uint8_t device_addr, uint16_t reg_addr, uint8_t value) {
		i2c_master_transfer_t masterXfer;
		memset(&masterXfer, 0, sizeof(masterXfer));

		masterXfer.slaveAddress = device_addr;
		masterXfer.direction = kI2C_Write;
		masterXfer.data = &value;
		masterXfer.dataSize = 1;
		masterXfer.flags = kI2C_TransferDefaultFlag;
		masterXfer.subaddress = reg_addr;

		if(MEMORY_ADDRESS == device_addr){
			masterXfer.subaddressSize = 2;
		}else if((RTC_ADDRESS == device_addr)||(ACCEL_ADDRESS == device_addr)){
			masterXfer.subaddress = (uint8_t) reg_addr;
			masterXfer.subaddressSize = 1;
		}
		I2C_MasterTransferNonBlocking(BOARD_I2C0_BASEADDR, get_I2CMasterHandle(), &masterXfer);
		vTaskDelay(100);
		/*  wait for transfer completed. */
		while ((!get_nakFlag()) && (!get_CompletionFlag())) {
		}

		set_nakFlag(false);

		if (get_CompletionFlag() == true) {
			set_CompletionFlag(false);
			return true;
		} else {
			return false;

		}
}

/*!
 * @brief I2C_ReadReg().
 *
 * Lee en el dispositivo I2C correspondiente, un registro determinado y lo almacena en una variable especificada.
 *
 */
bool I2C_ReadReg(I2C_Type *base, uint8_t device_addr, uint16_t reg_addr, uint8_t *rxBuff, uint32_t rxSize) {
		i2c_master_transfer_t masterXfer;
		memset(&masterXfer, 0, sizeof(masterXfer));
		masterXfer.slaveAddress = device_addr;
		masterXfer.direction = kI2C_Read;
		masterXfer.data = rxBuff;
		masterXfer.dataSize = rxSize;
		masterXfer.flags = kI2C_TransferDefaultFlag;
		masterXfer.subaddress = reg_addr;

		if(MEMORY_ADDRESS == device_addr){
			masterXfer.subaddressSize = 2;
		}else if((RTC_ADDRESS == device_addr)||(ACCEL_ADDRESS == device_addr)){
			masterXfer.subaddress = (uint8_t) reg_addr;
			masterXfer.subaddressSize = 1;
		}
		I2C_MasterTransferNonBlocking(BOARD_I2C0_BASEADDR, get_I2CMasterHandle(), &masterXfer);
		vTaskDelay(100);
		/*  wait for transfer completed. */
		while ((!get_nakFlag()) && (!get_CompletionFlag())) {
		}

		set_nakFlag(false);

		if (get_CompletionFlag() == true) {
			set_CompletionFlag(false);
			return true;
		} else {
			return false;
		}
}

/*!
 * @brief print_Hex().
 *
 * Imprime un string y un dato numerico en la terminal correspondiente
 *
 *@param conn: Es un parametro que recibe la direcion de la terminal con la cual se esta trabajando
 *@param msj: Recibe un string y lo manda a imprimir en la terminal correspondiente
 *@param var: Recibe un dato en HEX y lo manda a imprimir en la termianr correspondiente
 */
void print_Hex(struct netconn * conn,uint8_t* msj, uint8_t* var){
	if(NULL != msj){
		netconn_write(conn, (uint8_t *)msj,(uint8_t)strlen(msj), NETCONN_COPY);
	}
	if(NULL != var){
		char string[3] = {hexToASCII(*var/0x10), hexToASCII(*var%0x10),'\0'};
		uint8_t *stringPtr;
		stringPtr = string;
		netconn_write(conn, (uint8_t *)stringPtr, (uint8_t )strlen(stringPtr), NETCONN_COPY);
	}
}

/*!
 * @brief print_String().
 *
 * Imprime un string establecido y un string que el usuario ingreso o que puede variar
 *
 *@param conn: Es un parametro que recibe la direcion de la terminal con la cual se esta trabajando
 *@param msj: Recibe un string ya establecido y lo manda a imprimir en la terminal correspondiente
 *@param var: Recibe un string que puede variar y lo manda a imprimir en la termianr correspondiente
 */
void print_String(struct netconn * conn, uint8_t* msj, uint8_t* string){
	if(NULL != msj){
		netconn_write(conn, (uint8_t *)msj, (uint8_t )strlen(msj), NETCONN_COPY);
	}
	if(NULL != string){
		netconn_write(conn, (uint8_t *)string, (uint32_t )strlen(string), NETCONN_COPY);
	}
}

/*!
 * @brief int16ToString().
 *
 * Transforma un numero entero de 16 bits en un string
 *
 *@param number: Recibe el numero que se quiere transformar a string
*/
int8_t * int16ToString(int16_t number){
	uint8_t buffer[5];
	int8_t * string;
	int8_t i=4;
	uint8_t k = 0;
	uint8_t nZeros = 0;
	for(uint8_t m=0;m<5;m++){
		buffer[m] = 0;
	}
	if(number<0){
		number = sqrt(number*number);
		k++;
	}
	for(;i>=0;i--)
	{
		buffer[i] = number%10;
		number /= 10;
	}
	for(uint8_t j = 0;buffer[j]==0;j++){
		nZeros++;
	}
	if(k==1){
		buffer[0] = '-';
	}
	for(uint8_t j=nZeros; j<5 ;j++,k++){
		buffer[k] = buffer[j]+0x30;
		if(nZeros==1 && buffer[0] == '-'){}
		else buffer[j] = 0;
	}
	string = buffer;
	return string;
}

/*!
 * @brief hexToASCII().
 *
 * Transforma un numero hexadecimal a ASCII
 *
 *@param number: Recibe el numero que se quiere transformar a ASCII
*/
uint8_t hexToASCII(uint8_t n)
{
    if (n < 10) {
        return n + '0';
    } else {
        return (n - 10) + 'A';
    }
}

/*!
 * @brief BCDtoHex().
 *
 * Transforma un numero BCD a Hexadecimal
 *
 *@param number: Recibe el numero que se quiere transformar a Hexadecimal
*/
uint8_t BCDtoHex(uint8_t bcd)
{
	uint8_t dec=0;
	uint8_t mult;
    for (mult=1; bcd; bcd=bcd>>4,mult*=10)
    {
        dec += (bcd & 0x0f) * mult;
    }
    return dec;
}

/*!
 * @brief HexToBCD().
 *
 * Transforma un numero  Hexadecimal a BCD
 *
 *@param number: Recibe el numero que se quiere transformar a BCD
*/
uint8_t HexToBCD (uint8_t hex)
{
	uint8_t bcd;
	bcd = (hex / 10) << 4;
	bcd = bcd | (hex % 10);
    return bcd;
}

/*!
 * @brief DecToBCD().
 *
 * Transforma un numero  Decimal a BCD
 *
 *@param number: Recibe el numero que se quiere transformar a BCD
*/
uint8_t DecToBCD(uint8_t dec){
	uint8_t total = 0;
	uint8_t resultbase = 1;
	while(dec > 0 ){
		total += resultbase * (dec % 10);
		resultbase *= 16;
		dec /= 10;
	}
	return total;
}

/*!
 * @brief zerosBuffer().
 *
 * Ingresa a los buffers ceros para que no tenga basura y no se tenga problema de lectura
 *
 *@param buffer: Recibe el buffer donde se van a guardar los zeros
 *@param nElements: Recibe el numero de elementos que tiene el buffer
*/
void zerosBuffer(uint8_t * buffer,  uint8_t nElements){
	for(uint8_t i = 0;i<nElements;i++){
		buffer[i] = 0;
	}
}

/*!
 * @brief print_ERROR().
 *
 * Funcion para poder imprimir en la terminal que hubo un error
 *
 * @param conn: Es un parametro que recibe para determinar de que conexion fue la que se mando la informacion y a que
 * terminal se tiene que mandar la informacion
*/
void print_ERROR(struct netconn * conn){
	print_Hex(conn,menssage_error_1,NULL);
	print_Hex(conn,menssage_error_2,NULL);
    print_String(conn,ESC_string,NULL);
}

/*!
 * @brief printMenu().
 *
 * Funcion para poder imprimir en la terminal el menu que involucra las opciones establecidas
 *
 * @param conn: Es un parametro que recibe para determinar de que conexion fue la que se mando la informacion y a que
 * terminal se tiene que mandar la informacion
*/
void printMenu(struct netconn * conn){
	print_String(conn, Welcome_string, NULL);
	print_String(conn, Option0_string, NULL);
	print_String(conn, Option1_string, NULL);
	print_String(conn, Option2_string, NULL);
	print_String(conn, Option3_string, NULL);
	print_String(conn, Option4_string, NULL);
	print_String(conn, Option5_string, NULL);
	print_String(conn, Option6_string, NULL);
	print_String(conn, Option7_string, NULL);
	print_String(conn, Option8_string, NULL);
	print_String(conn, Option9_string, NULL);
	print_String(conn, Write_string, NULL);
}

/*!
 * @brief print_ESC().
 *
 * Funcion para poder decirle al usuario que tecla tiene que escribir para poder salir de las fucniones u opciones
 *
 * @param conn: Es un parametro que recibe para determinar de que conexion fue la que se mando la informacion y a que
 * terminal se tiene que mandar la informacion
*/
void print_ESC(struct netconn * conn){
    print_String(conn,ESC_string_2,NULL);
}

/*!
 * @brief print_BTN().
 *
 * Funcion que imprime en la terminal que para salir de la funcion periodica tien que presionar el push
 * botton externo
 *
 * @param conn: Es un parametro que recibe para determinar de que conexion fue la que se mando la informacion y a que
 * terminal se tiene que mandar la informacion
*/
void print_BTN(struct netconn * conn){
    print_String(conn,BTN_string,NULL);
}

/*!
 * @brief ADC_init().
 *
 * Funcion para inicializar el driver del ADC y pueda transformar la informacion analogica a digital
*/
void ADC_init(){
	adc16_channel_config_t channel_config;
	adc16_config_t adc_cnfg;

	adc_cnfg.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
	adc_cnfg.clockSource = kADC16_ClockSourceAlt0;
	adc_cnfg.enableContinuousConversion = false;
	adc_cnfg.clockDivider = kADC16_ClockDivider1;
	adc_cnfg.resolution = kADC16_ResolutionSE12Bit;
	adc_cnfg.longSampleMode = kADC16_LongSampleDisabled;
	adc_cnfg.enableHighSpeed = false;
	adc_cnfg.enableLowPower = false;
	adc_cnfg.enableContinuousConversion = 1;
	ADC16_Init(ADC0, &adc_cnfg);

	channel_config.channelNumber = 0x0C;
	channel_config.enableDifferentialConversion = false;
	channel_config.enableInterruptOnConversionCompleted = true;
	ADC16_SetChannelConfig(ADC0,0,&channel_config);
}

/*!
 * @brief updatedBits().
 *
 * Funcion para inicializar las bandera de los eventos que se utilizaran en las diferentes funciones y tareas creadas
 *
 * @param group: Es un parametro que recibe a que grupo se va a selecionar y modificar la bandera correspondiente
*/
EventBits_t updatedBits(uint8_t group) {
	return xEventGroupWaitBits(getEventGroup_Handler(group),
			Task_CERO | Task_ONE | Task_TWO | Task_THREE | Task_FOUR | Task_FIVE
					| Task_SIX | Task_SEVEN | Task_EIGHT | Task_NINE | USER_A
					| USER_B | USER_C | USER_D | USER_E,
			pdFALSE, pdFALSE, NO_DELAY);
}

/*!
 * @brief active().
 *
 * Funcion para activar las banderas de los eventos
 *
 * @param group: Es un parametro que recibe a que grupo se va a selecionar y modificar la bandera correspondiente
 * @param flag: Es un parametro que recibe que bandera es la que se va a modificar
*/
EventBits_t active(uint8_t group,uint32_t flag){
	EventBits_t bits;
	bits = updatedBits(group);
	xEventGroupSetBits(getEventGroup_Handler(group), flag);
	bits = updatedBits(group);
	return bits;
}

/*!
 * @brief notActive().
 *
 * Funcion para desactivar las banderas de los eventos
 *
 * @param group: Es un parametro que recibe a que grupo se va a selecionar y modificar la bandera correspondiente
 * @param flag: Es un parametro que recibe que bandera es la que se va a modificar
*/
EventBits_t notActive(uint8_t group,uint32_t flag){
	EventBits_t bits;
	bits = updatedBits(group);
	xEventGroupClearBits(getEventGroup_Handler(group), flag);
	bits = updatedBits(group);
	return bits;
}

/*******************************************************************************
 * GETTERS & SETTERS FUNCTIONS
 ******************************************************************************/
SemaphoreHandle_t getBotton1Sem(){
	return xBotton1_SemaphoreHandler;
}
void setBotton1Sem(SemaphoreHandle_t sem){
	xBotton1_SemaphoreHandler = sem;
}
SemaphoreHandle_t getMutex(){
	return xMutexHandler;
}
void setMutex(SemaphoreHandle_t mutex){
	xMutexHandler = mutex;
}

/*******************************************************************************
 * END
 ******************************************************************************/
