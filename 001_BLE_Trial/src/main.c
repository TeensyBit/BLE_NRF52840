#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

#define DEV_NAME  "dog"
#define DEV_NAME_SIZE   (sizeof(DEV_NAME)-1)

//prepare the advertising packet
static const struct bt_data ad[] = 
{
        //Set advertising flags
        BT_DATA_BYTES(BT_DATA_FLAGS,BT_LE_AD_NO_BREDR),        //Create a broadcaster with non-connectable advertising
        //Set advertising packet data
        BT_DATA(BT_DATA_NAME_COMPLETE,DEV_NAME,DEV_NAME_SIZE),
};

int main(void)
{
        return 0;
}
