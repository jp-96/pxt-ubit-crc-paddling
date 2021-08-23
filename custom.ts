
/**
* Use this file to define custom functions and blocks.
* Read more at https://makecode.microbit.org/blocks/custom
*/

/**
 * Custom blocks
 * icon: a Unicode identifier for an icon from the Font Awesome icon set.
 *       http://fontawesome.io/icons
 */
//% weight=100 color=#0fbc11 icon="\uf206"
namespace custom {
    
    /**
    * Start CSC service
    */
    //% blockId=custom_start_csc_service block="bluetooth csc service"
    //% parts="custom"
    //% shim=custom::startCSCService
    export function startCSCService(): void {
        console.log("startCSCService")
    }

    
    /**
     * Count up crank revolutions
     * 
     */
    //% blockId=custom_count_up_crank_revolutions block="count up crank revolutions"
    //% parts="custom"
    //% shim=custom::countUpCrankRevolutions
    export function countUpCrankRevolutions(): void {
        console.log("countUpCrankRevolutions")
    }
    
}
