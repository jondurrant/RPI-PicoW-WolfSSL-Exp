if (NOT DEFINED WOLFSSL_DIR)
    set(WOLFSSL_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/wolfssl")
endif()
if (NOT DEFINED WOLFSSL_PORT_DIR)
    set(WOLFSSL_PORT_DIR "${CMAKE_CURRENT_LIST_DIR}/port/wolfssl")
endif()

set(WOLFSSL_LWIP 1)

add_library(wolfssl STATIC)
target_sources(wolfssl PUBLIC

    ${WOLFSSL_DIR}/src/bio.c
    ${WOLFSSL_DIR}/src/crl.c
    ${WOLFSSL_DIR}/src/internal.c
    ${WOLFSSL_DIR}/src/keys.c
    ${WOLFSSL_DIR}/src/ocsp.c
    ${WOLFSSL_DIR}/src/sniffer.c
    ${WOLFSSL_DIR}/src/ssl.c
    ${WOLFSSL_DIR}/src/tls.c
    ${WOLFSSL_DIR}/src/tls13.c
    ${WOLFSSL_DIR}/src/wolfio.c

    ${WOLFSSL_DIR}/wolfcrypt/src/aes.c
    ${WOLFSSL_DIR}/wolfcrypt/src/cmac.c
    ${WOLFSSL_DIR}/wolfcrypt/src/des3.c
    ${WOLFSSL_DIR}/wolfcrypt/src/dh.c
    ${WOLFSSL_DIR}/wolfcrypt/src/ecc.c
    ${WOLFSSL_DIR}/wolfcrypt/src/hmac.c
    ${WOLFSSL_DIR}/wolfcrypt/src/random.c
    ${WOLFSSL_DIR}/wolfcrypt/src/rsa.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sha.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sha256.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sha512.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sha3.c

# wolfCrypt Additional
    ${WOLFSSL_DIR}/wolfcrypt/src/asm.c
    ${WOLFSSL_DIR}/wolfcrypt/src/asn.c
    ${WOLFSSL_DIR}/wolfcrypt/src/blake2s.c
    ${WOLFSSL_DIR}/wolfcrypt/src/chacha.c
    ${WOLFSSL_DIR}/wolfcrypt/src/chacha20_poly1305.c
    ${WOLFSSL_DIR}/wolfcrypt/src/coding.c
    ${WOLFSSL_DIR}/wolfcrypt/src/compress.c
    ${WOLFSSL_DIR}/wolfcrypt/src/cpuid.c
    ${WOLFSSL_DIR}/wolfcrypt/src/cryptocb.c
    ${WOLFSSL_DIR}/wolfcrypt/src/curve25519.c
    ${WOLFSSL_DIR}/wolfcrypt/src/curve448.c
    ${WOLFSSL_DIR}/wolfcrypt/src/ecc_fp.c
    ${WOLFSSL_DIR}/wolfcrypt/src/eccsi.c
    ${WOLFSSL_DIR}/wolfcrypt/src/ed25519.c
    ${WOLFSSL_DIR}/wolfcrypt/src/ed448.c
    ${WOLFSSL_DIR}/wolfcrypt/src/error.c
    ${WOLFSSL_DIR}/wolfcrypt/src/evp.c
    ${WOLFSSL_DIR}/wolfcrypt/src/fe_448.c
    ${WOLFSSL_DIR}/wolfcrypt/src/fe_low_mem.c
    ${WOLFSSL_DIR}/wolfcrypt/src/fe_operations.c
    ${WOLFSSL_DIR}/wolfcrypt/src/ge_448.c
    ${WOLFSSL_DIR}/wolfcrypt/src/ge_low_mem.c
    ${WOLFSSL_DIR}/wolfcrypt/src/ge_operations.c
    ${WOLFSSL_DIR}/wolfcrypt/src/hash.c
    ${WOLFSSL_DIR}/wolfcrypt/src/kdf.c
    ${WOLFSSL_DIR}/wolfcrypt/src/integer.c
    ${WOLFSSL_DIR}/wolfcrypt/src/logging.c
    ${WOLFSSL_DIR}/wolfcrypt/src/md5.c
    ${WOLFSSL_DIR}/wolfcrypt/src/memory.c
    ${WOLFSSL_DIR}/wolfcrypt/src/misc.c
    ${WOLFSSL_DIR}/wolfcrypt/src/pkcs12.c
    ${WOLFSSL_DIR}/wolfcrypt/src/pkcs7.c
    ${WOLFSSL_DIR}/wolfcrypt/src/poly1305.c
    ${WOLFSSL_DIR}/wolfcrypt/src/pwdbased.c
    ${WOLFSSL_DIR}/wolfcrypt/src/rc2.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sakke.c
    ${WOLFSSL_DIR}/wolfcrypt/src/signature.c
    ${WOLFSSL_DIR}/wolfcrypt/src/srp.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sp_arm32.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sp_arm64.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sp_armthumb.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sp_c32.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sp_c64.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sp_cortexm.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sp_dsp32.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sp_int.c
    ${WOLFSSL_DIR}/wolfcrypt/src/sp_x86_64.c
    ${WOLFSSL_DIR}/wolfcrypt/src/tfm.c
    ${WOLFSSL_DIR}/wolfcrypt/src/wc_dsp.c
    ${WOLFSSL_DIR}/wolfcrypt/src/wc_encrypt.c
    ${WOLFSSL_DIR}/wolfcrypt/src/wc_pkcs11.c
    ${WOLFSSL_DIR}/wolfcrypt/src/wc_port.c
    ${WOLFSSL_DIR}/wolfcrypt/src/wolfevent.c
    ${WOLFSSL_DIR}/wolfcrypt/src/wolfmath.c
    
    ${WOLFSSL_PORT_DIR}/myTime.c

)

target_include_directories(wolfssl PUBLIC 
	${WOLFSSL_DIR}
	${WOLFSSL_PORT_DIR}
	${PICO_SDK_PATH}/lib/lwip/src/include/compat/posix
)

# Add the standard library to the build
target_link_libraries(wolfssl PUBLIC 
    pico_stdlib 
    pico_cyw43_arch_lwip_sys_freertos
    FreeRTOS-Kernel
    FREERTOS_PORT
    LWIP_PORT
    hardware_adc
    )


target_compile_definitions(wolfssl PUBLIC
    WOLFSSL_LWIP=1
    USE_WOLF_ARM_STARTUP=1
    WOLFSSL_USER_SETTINGS=1
    NO_WRITEV=1
)
