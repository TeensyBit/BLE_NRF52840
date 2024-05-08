#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

#define DEV_NAME  "rat"
#define DEV_NAME_SIZE   (sizeof(DEV_NAME)-1)

static unsigned char url_data[] ={0x17,'/','/','a','c','a','d','e','m','y','.',
                                 'n','o','r','d','i','c','s','e','m','i','.',
                                 'c','o','m'};

//prepare the advertising packet
static const struct bt_data ad[] = 
{
        //Set advertising flags
        BT_DATA_BYTES(BT_DATA_FLAGS,BT_LE_AD_NO_BREDR),        //Create a broadcaster with non-connectable advertising
        //Set advertising packet data
        BT_DATA(BT_DATA_NAME_COMPLETE,DEV_NAME,DEV_NAME_SIZE),
};

//prepare the advertising packet
static const struct bt_data sd[] = 
{
        // Include the URL data in the scan response packet
        BT_DATA(BT_DATA_URI, url_data, sizeof(url_data)),
};

int main(void)
{      
        int err = bt_enable(NULL); //Enable BT
        err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));	  //Start Advertising
        /*if (err) 
        {
                LOG_ERR("Advertising failed to start (err %d)\n", err);		
                return -1;	
        }*/
        return 0;
}