#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <dk_buttons_and_leds.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/conn.h>

#define DEV_NAME                "rat"
#define DEV_NAME_SIZE           (sizeof(DEV_NAME)-1)
#define COMPANY_ID_CODE         0x1234
#define USER_BUTTON             DK_BTN1_MSK
#define CONN_STAUS_LED          DK_LED2

void on_connected(struct bt_conn *conn, uint8_t err);
void on_disconnected(struct bt_conn *conn, uint8_t reason);

static unsigned char url_data[] ={0x17,'/','/','a','c','a','d','e','m','y','.',
                                 'n','o','r','d','i','c','s','e','m','i','.',
                                 'c','o','m'};


//Declare callbacks
struct bt_conn_cb conn_callbacks = 
{
        .connected = on_connected,
        .disconnected = on_disconnected,
};

//Callback functions
void on_connected(struct bt_conn *conn, uint8_t err)
{       
        printk("Connected\n");
        dk_set_led(CONN_STAUS_LED, 1);
}
void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
        printk("Disconnected\n");
        dk_set_led(CONN_STAUS_LED, 0);
}

//Control advertising params
static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM((BT_LE_ADV_OPT_CONNECTABLE|BT_LE_ADV_OPT_USE_IDENTITY), 800, 801, NULL);

//declare the struct for custom data
typedef struct adv_mfg_data
{
      uint16_t company_code;
      uint16_t number_press;  
} adv_mfg_data_type;

static adv_mfg_data_type  adv_mfg_data = {COMPANY_ID_CODE, 0x00};

//prepare the advertising packet
static const struct bt_data ad[] = 
{
        //Set advertising flags
        BT_DATA_BYTES(BT_DATA_FLAGS,(BT_LE_AD_GENERAL|BT_LE_AD_NO_BREDR)),      
        //Set advertising packet data
        BT_DATA(BT_DATA_NAME_COMPLETE,DEV_NAME,DEV_NAME_SIZE),
        BT_DATA(BT_DATA_MANUFACTURER_DATA,(unsigned char*)&adv_mfg_data,sizeof(adv_mfg_data)),
};

//prepare the scan response packet
static const struct bt_data sd[] = 
{
        // Include the URL data in the scan response packet
        //BT_DATA(BT_DATA_URI, url_data, sizeof(url_data)),
        BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)),
};

static void button_changed(uint32_t button_state, uint32_t has_changed);

//Buttons library 
static int init_button(void)
{
        int err;
        err = dk_buttons_init(button_changed);
        return err;
}

static void button_changed(uint32_t button_state, uint32_t has_changed)
{
        if(has_changed & button_state & USER_BUTTON)
        {
                adv_mfg_data.number_press+=1;
                bt_lbs_send_button_state(true);
                bt_le_adv_update_data(ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
        }
        else
                bt_lbs_send_button_state(false);
}

void config_rsa(void)
{
        int err;
        bt_addr_le_t addr;
        err = bt_addr_le_from_str("FF:EE:DD:CC:BB:AA","random",&addr);
        err = bt_id_create(&addr,NULL);
}

int main(void)
{     
        dk_leds_init(); 
        init_button();
        config_rsa(); //config static address 
        bt_conn_cb_register(&conn_callbacks);   //register callback

        int err = bt_enable(NULL); //Enable BT
        err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));	  //Start Advertising
        /*if (err) 
        {
                LOG_ERR("Advertising failed to start (err %d)\n", err);		
                return -1;	
        }*/
        return 0;
}