
/**
* Use this file to define custom functions and blocks.
* Read more at https://makecode.microbit.org/blocks/custom
*/

/**
 * Custom blocks
 * icon: a Unicode identifier for an icon from the Font Awesome icon set.
 *       http://fontawesome.io/icons
 */
//% weight=100 color=#0fbc11 icon=""
namespace custom {
    
    /**
    * Start CSC controller.
    */
    //% block
    //% shim=custom_cpp::startCSCService
    export function startCSCService(): void {
        console.log("startCSCService")
    }

    
    /**
     * Count up crank revolutions
     * 
     */
    //% block
    //% shim=custom_cpp::countUpCrankRevolutions
    export function countUpCrankRevolutions(): void {
        console.log("countUpCrankRevolutions")
    }
    
}
