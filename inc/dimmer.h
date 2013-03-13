/**
 * @file
 * @brief define MACRO.
 * @author Zeng Xuedong
 * @date 2012-10-20
 */
#ifndef __DIMMER__
#define __DIMMER__

/** @def MAX_RCV_NUM
 * @brief Number of I2C receive buffer
 */
#define MAX_RCV_NUM 11

/** @def GROUP_SEL
 * @brief I2C register accessed through I2C interface<br>
 * Select specify light join to group for lighting level control, and switch on/off light individually
 * @details
 * <TABLE>
 * <TR>
 * 		<TH colspan="8">Address:0x00 R/W RESET:0x00
 * </TR>
 * <TR>
 * <TH>b7</TH><TH>b6</TH><TH>b5</TH><TH>b4</TH><TH>b3</TH><TH>b2</TH><TH>b1</TH><TH>b0</TH>
 * </TR>
 * <TR>
 * <TH>p4</TH><TH>p3</TH><TH>p2</TH><TH>p1</TH><TH>s4</TH><TH>s3</TH><TH>s2</TH><TH>s1</TH>
 * </TR>
 * </TABLE>
 * Bit
 * <HR>
 * 7:power switch for light 4, 1 = ON, 0 = OFF<br>
 * 6:power switch for light 3, 1 = ON, 0 = OFF<br>
 * 5:power switch for light 2, 1 = ON, 0 = OFF<br>
 * 4:power switch for light 1, 1 = ON, 0 = OFF<br>
 * 3:group selector for light 4, 1 = join group, 0 = exclusive<br>
 * 2:group selector for light 3, 1 = join group, 0 = exclusive<br>
 * 1:group selector for light 2, 1 = join group, 0 = exclusive<br>
 * 0:group selector for light 1, 1 = join group, 0 = exclusive<br>
 * The light join to group can effective by group control register ABS_ALL and INC_ALL
 */
#define GROUP_SEL   0

/** @def ABS_ALL
 * @brief I2C register accessed through I2C interface<br>
 * Set absolute lighting for all lights selected in GROUP_SEL register
 * @details
* <TABLE>
 * <TR>
 * 		<TH colspan="8">Address:0x01 R/W RESET:0x00
 * </TR>
 * <TR>
 * <TH>b7</TH><TH>b6</TH><TH>b5</TH><TH>b4</TH><TH>b3</TH><TH>b2</TH><TH>b1</TH><TH>b0</TH>
 * </TR>
 * <TR>
 * <TH>AL7</TH><TH>AL6</TH><TH>AL5</TH><TH>AL4</TH><TH>AL3</TH><TH>AL2</TH><TH>AL1</TH><TH>AL0</TH>
 * </TR>
 * </TABLE>
 * Bit
 * <HR>
 * 7:0 AL[7:0] Set absolute lighting level for all that selected by GROUP_SEL register, the value range: 0-100
 */
#define ABS_ALL     1

/** @def INC_ALL
 * @brief I2C register accessed through I2C interface<br>
 * Set increment of lighting level for all lights selected in GROUP_SEL register
 * @details
* <TABLE>
 * <TR>
 * 		<TH colspan="8">Address:0x02 R/W RESET:0x00
 * </TR>
 * <TR>
 * <TH>b7</TH><TH>b6</TH><TH>b5</TH><TH>b4</TH><TH>b3</TH><TH>b2</TH><TH>b1</TH><TH>b0</TH>
 * </TR>
 * <TR>
 * <TH>IL7</TH><TH>IL6</TH><TH>IL5</TH><TH>IL4</TH><TH>IL3</TH><TH>IL2</TH><TH>IL1</TH><TH>IL0</TH>
 * </TR>
 * </TABLE>
 * Bit
 * <HR>
 * 7:0 IL[7:0] Increment light, range -100-+100,binary complementary, the value will add to the
 * absolute lighting level register that selected by GROUP_SEL. If the result out of range 0-100, it will be trimmed to 0-100
 */
#define INC_ALL     2

/** @def ABS_G0
 * @brief I2C register accessed through I2C interface<br>
 * Set absolute lighting for light 1
 * @details
 * <TABLE>
 * <TR>
 * 		<TH colspan="8">Address:0x03 R/W RESET:0x00
 * </TR>
 * <TR>
 * <TH>b7</TH><TH>b6</TH><TH>b5</TH><TH>b4</TH><TH>b3</TH><TH>b2</TH><TH>b1</TH><TH>b0</TH>
 * </TR>
 * <TR>
 * <TH>L7</TH><TH>L6</TH><TH>L5</TH><TH>L4</TH><TH>L3</TH><TH>L2</TH><TH>L1</TH><TH>L0</TH>
 * </TR>
 * </TABLE>
 * Bit
 * <HR>
 * 7:0 L[7:0] Set absolute lighting level for light 1, range 0-100
 */
#define ABS_G0      3

/** @def INC_G0
 * @brief I2C register accessed through I2C interface<br>
 * Set increment of lighting level for light 1
 * @details
 * <TABLE>
 * <TR>
 * 		<TH colspan="8">Address:0x04 R/W RESET:0x00
 * </TR>
 * <TR>
 * <TH>b7</TH><TH>b6</TH><TH>b5</TH><TH>b4</TH><TH>b3</TH><TH>b2</TH><TH>b1</TH><TH>b0</TH>
 * </TR>
 * <TR>
 * <TH>IL7</TH><TH>IL6</TH><TH>IL5</TH><TH>IL4</TH><TH>IL3</TH><TH>IL2</TH><TH>IL1</TH><TH>IL0</TH>
 * </TR>
 * </TABLE>
 * Bit
 * <HR>
 * 7:0 IL[7:0] Increment light, range -100-+100,binary complementary, the value will add to the
 * absolute lighting level register of light 1. If the result out of range 0-100, it will be trimmed to 0-100
 */
#define INC_G0      4

/** @def ABS_G1
 * @brief I2C register accessed through I2C interface<br>
 * Set absolute lighting level for light 2
 * @details
 * <TABLE>
 * <TR>
 * 		<TH colspan="8">Address:0x05 R/W RESET:0x00
 * </TR>
 * <TR>
 * <TH>b7</TH><TH>b6</TH><TH>b5</TH><TH>b4</TH><TH>b3</TH><TH>b2</TH><TH>b1</TH><TH>b0</TH>
 * </TR>
 * <TR>
 * <TH>L7</TH><TH>L6</TH><TH>L5</TH><TH>L4</TH><TH>L3</TH><TH>L2</TH><TH>L1</TH><TH>L0</TH>
 * </TR>
 * </TABLE>
 * Bit
 * <HR>
 * 7:0 L[7:0] Set absolute lighting level for light 2, range 0-100
 */
#define ABS_G1      5

/** @def INC_G1
 * @brief I2C register accessed through I2C interface<br>
 * Set increment of lighting level for light 2
 * @details
 * <TABLE>
 * <TR>
 * 		<TH colspan="8">Address:0x06 R/W RESET:0x00
 * </TR>
 * <TR>
 * <TH>b7</TH><TH>b6</TH><TH>b5</TH><TH>b4</TH><TH>b3</TH><TH>b2</TH><TH>b1</TH><TH>b0</TH>
 * </TR>
 * <TR>
 * <TH>IL7</TH><TH>IL6</TH><TH>IL5</TH><TH>IL4</TH><TH>IL3</TH><TH>IL2</TH><TH>IL1</TH><TH>IL0</TH>
 * </TR>
 * </TABLE>
 * Bit
 * <HR>
 * 7:0 IL[7:0] Increment light, range -100-+100,binary complementary, the value will add to the
 * absolute lighting level register of light 2. If the result out of range 0-100, it will be trimmed to 0-100
 */
#define INC_G1      6

/** @def ABS_G2
 * @brief I2C register accessed through I2C interface<br>
 * Set absolute lighting level for light 3
 * @details
 * <TABLE>
 * <TR>
 * 		<TH colspan="8">Address:0x07 R/W RESET:0x00
 * </TR>
 * <TR>
 * <TH>b7</TH><TH>b6</TH><TH>b5</TH><TH>b4</TH><TH>b3</TH><TH>b2</TH><TH>b1</TH><TH>b0</TH>
 * </TR>
 * <TR>
 * <TH>L7</TH><TH>L6</TH><TH>L5</TH><TH>L4</TH><TH>L3</TH><TH>L2</TH><TH>L1</TH><TH>L0</TH>
 * </TR>
 * </TABLE>
 * Bit
 * <HR>
 * 7:0 L[7:0] Set absolute lighting level for light 3, range 0-100
 */
#define ABS_G2      7

/** @def INC_G2
 * @brief I2C register accessed through I2C interface<br>
 * Set increment of lighting level for light 3
 * @details
 * <TABLE>
 * <TR>
 * 		<TH colspan="8">Address:0x08 R/W RESET:0x00
 * </TR>
 * <TR>
 * <TH>b7</TH><TH>b6</TH><TH>b5</TH><TH>b4</TH><TH>b3</TH><TH>b2</TH><TH>b1</TH><TH>b0</TH>
 * </TR>
 * <TR>
 * <TH>IL7</TH><TH>IL6</TH><TH>IL5</TH><TH>IL4</TH><TH>IL3</TH><TH>IL2</TH><TH>IL1</TH><TH>IL0</TH>
 * </TR>
 * </TABLE>
 * Bit
 * <HR>
 * 7:0 IL[7:0] Increment light, range -100-+100,binary complementary, the value will add to the
 * absolute lighting level register of light 3. If the result out of range 0-100, it will be trimmed to 0-100
 */
#define INC_G2      8

/** @def ABS_G3
 * @brief I2C register accessed through I2C interface<br>
 * Set absolute lighting level for light 4
 * @details
 * <TABLE>
 * <TR>
 * 		<TH colspan="8">Address:0x09 R/W RESET:0x00
 * </TR>
 * <TR>
 * <TH>b7</TH><TH>b6</TH><TH>b5</TH><TH>b4</TH><TH>b3</TH><TH>b2</TH><TH>b1</TH><TH>b0</TH>
 * </TR>
 * <TR>
 * <TH>L7</TH><TH>L6</TH><TH>L5</TH><TH>L4</TH><TH>L3</TH><TH>L2</TH><TH>L1</TH><TH>L0</TH>
 * </TR>
 * </TABLE>
 * Bit
 * <HR>
 * 7:0 L[7:0] Set absolute lighting level for light 4, range 0-100
 */
#define ABS_G3      9

/** @def INC_G3
 * @brief I2C register accessed through I2C interface<br>
 * Set increment of lighting level for light 4
 * @details
 * <TABLE>
 * <TR>
 * 		<TH colspan="8">Address:0x0A R/W RESET:0x00
 * </TR>
 * <TR>
 * <TH>b7</TH><TH>b6</TH><TH>b5</TH><TH>b4</TH><TH>b3</TH><TH>b2</TH><TH>b1</TH><TH>b0</TH>
 * </TR>
 * <TR>
 * <TH>IL7</TH><TH>IL6</TH><TH>IL5</TH><TH>IL4</TH><TH>IL3</TH><TH>IL2</TH><TH>IL1</TH><TH>IL0</TH>
 * </TR>
 * </TABLE>
 * Bit
 * <HR>
 * 7:0 IL[7:0] Increment light, range -100-+100,binary complementary, the value will add to the
 * absolute lighting level register of light 4. If the result out of range 0-100, it will be trimmed to 0-100
 */
#define INC_G3      10

#define GROUPSEL   1
#define ABSALL     2
#define INCALL     3
#define ABSG0      4
#define INCG0      5
#define ABSG1      6
#define INCG1      7
#define ABSG2      8
#define INCG2      9
#define ABSG3      10
#define INCG3      11

#define PW_50HZ      960   //1200 * 0.8
#define PW_60HZ      749   //936 * 0.8

void Setup_USI_Slave(void);
void init_flash(void);
void save_sta(void);
void erase_flash(void);
#endif
