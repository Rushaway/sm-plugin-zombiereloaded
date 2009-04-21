/*
 * ============================================================================
 *
 *   Zombie:Reloaded
 *
 *   File:        hitgroup.inc
 *   Description: API for loading hitgroup specific settings.
 *
 * ============================================================================
 */

/**
 * Array to store keyvalue data.
 */
new Handle:kvHitgroups = INVALID_HANDLE;

/**
 * @section Player hitgroup values.
 */
#define	HITGROUP_GENERIC    0
#define	HITGROUP_HEAD       1
#define	HITGROUP_CHEST      2
#define	HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4
#define HITGROUP_RIGHTARM   5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG   7
#define HITGROUP_GEAR       10
/**
 * @endsection
 */

/**
 * Clears hitgroup data.
 */ 
HitgroupsClearData()
{
    // Load hitgroup data.
    if (kvHitgroups != INVALID_HANDLE)
    {
        CloseHandle(kvHitgroups);
    }
    
    kvHitgroups = CreateKeyValues("hitgroups");
}

/**
 * Loads hitgroup data from file.
 */ 
HitgroupsLoad()
{
    // Clear hitgroup data
    HitgroupsClearData();
    
    // If module is disabled, then stop.
    new bool:hitgroups = GetConVarBool(g_hCvarsList[CVAR_HITGROUPS]);
    if (!hitgroups)
    {
        return;
    }
    
    decl String:path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, path, sizeof(path), "configs/zr/hitgroups.txt");
    
    // If file isn't found, stop plugin.
    if (!FileToKeyValues(kvHitgroups, path))
    {
        if (LogCheckFlag(LOG_CORE_EVENTS, LOG_MODULE_HITGROUPS))
        {
            LogMessageFormatted(-1, "Hitgroups", "Config Validation", "Missing file hitgroups.txt, disabling hitgroup-based modules.", LOG_FORMAT_TYPE_FULL);
        }
        
        return;
    }
    
    // Validate hitgroups config.
    HitgroupsValidateConfig();
}

/**
 * Validate hitgroup config file and settings.
 */ 
HitgroupsValidateConfig()
{
    // If log flag check fails, don't log.
    if (!LogCheckFlag(LOG_CORE_EVENTS, LOG_MODULE_HITGROUPS))
    {
        return;
    }
    
    KvRewind(kvHitgroups);
    if (!KvGotoFirstSubKey(kvHitgroups))
    {
        LogMessageFormatted(-1, "Hitgroups", "Config Validation", "No hitgroups listed in hitgroups.txt, disabling hitgroup-based modules.", LOG_FORMAT_TYPE_FULL);
    }
}

/**
 * Retrieve hitgroup knockback value.
 * 
 * @param hitgroup  The hitgroup index.
 */   
Float:HitgroupsGetHitgroupKnockback(hitgroup)
{
    // Reset keyvalue's traversal stack.
    KvRewind(kvHitgroups);
    if (KvGotoFirstSubKey(kvHitgroups))
    {
        decl String:sHitgroup[4];
        
        do
        {
            KvGetSectionName(kvHitgroups, sHitgroup, sizeof(sHitgroup));
            
            // If this is the right hitgroup, then return knockback for it.
            if (hitgroup == StringToInt(sHitgroup))
            {
                return KvGetFloat(kvHitgroups, "knockback", 1.0);
            }
        } while (KvGotoNextKey(kvHitgroups));
    }
    
    return 1.0;
}