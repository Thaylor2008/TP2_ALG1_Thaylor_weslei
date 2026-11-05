import os
import time
import matplotlib.pyplot as plt

# Pastas dos testes
pasta_entradas = os.path.join("testes", "entradas")

# Lista para armazenar resultados: (nome_caso, tamanho_W, num_Q, tempo_guloso, tempo_divconq)
resultados = []

print("Iniciando medições de tempo COMPLETAS (isso pode levar cerca de 1 hora)...")

# Percorre todos os 31 arquivos casoXX.in ordenados numericamente
arquivos_in = sorted(
    [f for f in os.listdir(pasta_entradas) if f.endswith(".in")],
    key=lambda x: int(''.join(filter(str.isdigit, x)) or 0)
)

total_testes = len(arquivos_in)

for idx, arq in enumerate(arquivos_in, 1):
    caminho_in = os.path.join(pasta_entradas, arq)
    
    # Lê H, W e Q do arquivo de entrada
    with open(caminho_in, 'r') as f:
        linhas = f.readlines()
        H, W = map(int, linhas[0].split())
        Q = int(linhas[H + 1].strip())

    nome_caso = arq.replace(".in", "")

    print(f"\n[{idx}/{total_testes}] Processando {nome_caso} (W={W}, Q={Q})...")

    # --- Medição 1: Divisão e Conquista ---
    t0 = time.perf_counter()
    os.system(f".\\tp02-divconq.exe < \"{caminho_in}\" > nul")
    t1 = time.perf_counter()
    tempo_divconq = t1 - t0

    # --- Medição 2: Guloso (Executando todos os casos) ---
    print(f" -> Rodando Guloso no {nome_caso}...")
    t0 = time.perf_counter()
    os.system(f".\\tp02-guloso.exe < \"{caminho_in}\" > nul")
    t1 = time.perf_counter()
    tempo_guloso = t1 - t0

    resultados.append((nome_caso, W, Q, tempo_guloso, tempo_divconq))
    
    print(f" Concluído {nome_caso} | DivConq: {tempo_divconq:.4f}s | Guloso: {tempo_guloso:.4f}s")

# --- GERAR GRÁFICO 1: Comparação Completa (Todos os 31 Casos em Escala Logarítmica) ---
nomes_todos = [r[0] for r in resultados]
t_guloso_todos = [r[3] for r in resultados]
t_divconq_todos = [r[4] for r in resultados]

plt.figure(figsize=(12, 6))
plt.plot(nomes_todos, t_guloso_todos, marker='o', color='red', label='Guloso (Dijkstra Direto)', linewidth=2)
plt.plot(nomes_todos, t_divconq_todos, marker='s', color='blue', label='Divisão e Conquista', linewidth=2)
plt.yscale('log') # Escala logarítmica crucial para comparar tempos muito distantes
plt.xlabel('Casos de Teste')
plt.ylabel('Tempo de Execução em Segundos (Escala Log)')
plt.title('Comparação Completa de Desempenho (31 Casos de Teste)')
plt.xticks(rotation=45, ha='right')
plt.grid(True, which="both", ls="--", alpha=0.5)
plt.legend()
plt.tight_layout()
plt.savefig('grafico_comparacao_completa.png', dpi=300)
plt.close()

# --- GERAR GRÁFICO 2: Escalabilidade em Escala Linear ---
plt.figure(figsize=(12, 6))
plt.plot(nomes_todos, t_guloso_todos, marker='o', color='red', label='Guloso (Dijkstra Direto)', linewidth=2)
plt.plot(nomes_todos, t_divconq_todos, marker='s', color='blue', label='Divisão e Conquista', linewidth=2)
plt.xlabel('Casos de Teste')
plt.ylabel('Tempo de Execução em Segundos (Escala Linear)')
plt.title('Impacto do Crescimento de W e Q no Tempo de Resposta')
plt.xticks(rotation=45, ha='right')
plt.grid(True, ls="--", alpha=0.5)
plt.legend()
plt.tight_layout()
plt.savefig('grafico_escalabilidade_linear.png', dpi=300)
plt.close()

print("\n==========================================")
print("Todas as medições foram concluídas com sucesso!")
print("Gráficos gerados:")
print(" 1. grafico_comparacao_completa.png")
print(" 2. grafico_escalabilidade_linear.png")
print("==========================================")