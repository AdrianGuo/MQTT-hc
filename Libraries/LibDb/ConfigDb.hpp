/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ConfigDb.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 26 Dec 2016 22:00:35
 *
 ******************************************************************************/
#ifndef CONFIG_DB_HPP_
#define CONFIG_DB_HPP_

#ifdef MT7688
#ifndef PATH_RAM
#define PATH_RAM        ("/tmp/")
#endif /* PATH_RAM */

#ifndef PATH_FLASH
#define PATH_FLASH      ("/Lumi/")
#endif /* PATH_FLASH */
#else /* MT7688 */
#ifndef PATH_RAM
#define PATH_RAM        ("/home/osboxes/git/newhc/")
#endif /* PATH_RAM */

#ifndef PATH_FLASH
#define PATH_FLASH      ("/home/osboxes/git/")
#endif /* PATH_FLASH */
#endif /* MT7688 */

#endif /* CONFIG_DB_HPP_ */
