# Embedded DSP on STM32F4 - Bare Metal Implementation

A digital signal processing project on STM32F4 using bare metal programming with custom drivers and MATLAB-generated filter coefficients.

## 🔧 Technical Implementation Workflow

### 1. ADC Driver Development

Custom bare metal ADC driver for PA1 analog input:

```c
// ADC Configuration
void pa1_adc_init(void);
uint32_t adc_read(void);
void start_conversion(void);
```

**Key Features:**
- PA1 pin configuration for analog input
- 12-bit resolution, right-aligned data
- Continuous conversion mode
- Non-blocking read operations
- CMSIS register-level programming

### 2. System Clock Configuration (100MHz)

Custom clock tree configuration achieving 100MHz system clock:

```c
void clock_100Mhz_config(void);
```

**Clock Parameters:**
- HSI/HSE → PLL configuration
- SYSCLK: 100MHz
- AHB: 100MHz  
- APB1: 50MHz (PCLK1)
- APB2: 100MHz (PCLK2)
- Optimized for DSP operations with FPU enabled

### 3. FIR Filter Implementation

Real-time FIR filtering structure and functions:

```c
typedef struct {
    float32_t *coeffs;
    float32_t *buffer;
    uint32_t length;
    uint32_t index;
} fir_filter_t;

void fir_fiter_init(fir_filter_t *filter, float32_t *coeffs, uint32_t length);
float32_t fir_filter_update(fir_filter_t *filter, float32_t input);
```

**Implementation:**
- Circular buffer for delay line
- Single-sample real-time processing
- Optimized for embedded memory constraints
- Direct form implementation

### 4. FIFO Buffer Structure Development

Background batch processing buffer system:

```c
typedef struct {
    float32_t data;
    // Additional metadata if needed
} rx_data_t;

// FIFO Operations
void rx_fifo_init(void);
uint8_t rx_fifo_put(rx_data_t data);
uint8_t rx_fifo_get(rx_data_t *data);
```

**Features:**
- Circular buffer implementation
- Thread-safe with flag-based synchronization
- Configurable size (RXFIFOSIZE)
- Full/empty state detection
- Designed for interrupt-driven data collection

### 5. TIM2 Background Processing Driver

Timer-based interrupt system for batch DSP processing:

```c
void tim2_1hz_interrupt_init(void);
void TIM2_IRQHandler(void);
static void tim2_callback(void);
```

**Configuration:**
- 1Hz interrupt rate for batch processing
- Manages FIFO fill/process cycle
- Controls `g_process_flag` and `g_fifo_full_flag`
- Coordinates ADC sampling with DSP processing

**Processing Flow:**
```
TIM2 IRQ (1Hz) → Fill FIFO → Set Process Flag → DSP Processing → Reset Flag
```

### 6. DSP Filter Implementations

Three filtering approaches implemented:

#### A. Convolution-Based Filtering
```c
void calc_convolution(float32_t *sig_src_arr,
                     float32_t *sig_dest_arr,
                     float32_t *imp_response_arr,
                     uint32_t signal_length,
                     uint32_t impulse_length);
```

#### B. Moving Average Filter
```c
void moving_average(float32_t *sig_src_arr,
                   float32_t *sig_out_arr,
                   uint32_t signal_length,
                   uint32_t filter_pts);
```

**Parameters:**
- Filter points: 11 (configurable)
- Symmetric window implementation
- Edge handling with floor(filter_pts/2)

### 7. MATLAB SDA Tool Integration

Filter coefficient generation parameters:

#### Filter Design Specifications:
```matlab
% Low-pass Filter Parameters
Sampling_Frequency = 100;    % Hz
Passband_Frequency = 1;      % Hz  
Stopband_Frequency = 3;      % Hz
Passband_Ripple = 1;         % dB
Stopband_Attenuation = 60;   % dB
```

#### Generated Coefficients:
- `LP_1HZ_2HZ8IMPULSE_RESPONSE[IMP_RESP_LENGTH]`
- `LP_1HZ_3HZ_IMPULSE_RESPONSE[IMP_RESP2_LENGTH]`

#### SDA Tool Settings:
- **Filter Type:** FIR Low-pass
- **Design Method:** Parks-McClellan (Remez)
- **Window:** None (optimal design)
- **Coefficient Format:** float32_t
- **Quantization:** Full precision

## 🎯 System Architecture

```
ADC (PA1) → FIFO Buffer → TIM2 Trigger → Batch Processing
    ↓                                            ↓
Real-time FIR ←→ Background Processing → [Convolution/Moving Average]
```

## 📊 Technical Specifications

| Component | Specification |
|-----------|---------------|
| **MCU** | STM32F4xx @ 100MHz |
| **ADC** | 12-bit, continuous mode, PA1 |
| **Processing** | ARM CMSIS DSP, hardware FPU |
| **Sampling** | TIM2-controlled, 1Hz batch |
| **Filters** | MATLAB SDA generated coefficients |
| **Memory** | Static allocation, no HAL |

## 🔍 Key Implementation Details

### FPU Activation
```c
// Enable CP10 and CP11 full access
SCB->CPACR |= ((3UL << 10*2)) | ((3UL << 10*2));
```

### Processing Synchronization
- `g_fifo_full_flag`: Controls FIFO fill state
- `g_process_flag`: Triggers batch DSP processing
- Interrupt-driven data flow management

### Memory Management
- `sensor_data_buffer[RXFIFOSIZE]`: Input buffer
- `output_signal_arr[INPUT_SIG_LEN]`: Processed output
- `INPUT_SIG_LEN = RXFIFOSIZE + IMP_RESP2_LENGTH - 1`

---

**Development Focus:** Bare metal embedded DSP with optimized real-time performance and MATLAB-designed filter integration.
