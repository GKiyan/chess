# Nom du compilateur
CXX = g++

# Options de compilation
CXXFLAGS = -Iinclude -Wall -Wextra -O2 -DDEBUG -fopenmp

# Options de l'édition des liens
LDFLAGS = -fopenmp

# Dossiers et fichiers sources
SRC_DIR = src
TEST_DIR = test
SRC = $(SRC_DIR)/App.cpp \
      $(SRC_DIR)/chess/piece/Piece.cpp \
      $(SRC_DIR)/chess/board/Board.cpp \
      $(SRC_DIR)/chess/Chess.cpp \
      $(SRC_DIR)/chess/algorithms/Algorithm.cpp \
      $(SRC_DIR)/chess/algorithms/heuristics/Heuristics.cpp

TEST_SRC = $(TEST_DIR)/chess/Chess.test.cpp \
           $(TEST_DIR)/chess/board/Board.test.cpp

# Objets générés
OBJ = $(SRC:.cpp=.o)

# Nom de l'exécutable
EXECUTABLE = app

# Nom de l'exécutable de test
TEST_EXECUTABLE = test.o

# Commande par défaut
all: $(EXECUTABLE)

# Règle de compilation de l'exécutable
$(EXECUTABLE): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Règle générique de compilation des fichiers .cpp en .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle pour exécuter Valgrind sur l'application
valgrind: $(EXECUTABLE)
	valgrind --leak-check=yes ./$(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Section pour les tests
test: test_chess test_board

# Règle pour tester chess/Chess
test_chess: $(TEST_DIR)/chess/Chess.test.cpp $(SRC_DIR)/chess/Chess.cpp $(SRC_DIR)/chess/board/Board.cpp $(SRC_DIR)/chess/piece/Piece.cpp $(SRC_DIR)/chess/algorithms/Algorithm.cpp
	$(CXX) -o $(TEST_EXECUTABLE) $(TEST_DIR)/chess/Chess.test.cpp $(SRC_DIR)/chess/Chess.cpp $(SRC_DIR)/chess/board/Board.cpp $(SRC_DIR)/chess/piece/Piece.cpp $(SRC_DIR)/chess/algorithms/Algorithm.cpp
	valgrind --leak-check=yes ./$(TEST_EXECUTABLE)
	rm -f $(TEST_EXECUTABLE)

# Règle pour tester chess/board/Board
test_board: $(TEST_DIR)/chess/board/Board.test.cpp $(SRC_DIR)/chess/Chess.cpp $(SRC_DIR)/chess/board/Board.cpp $(SRC_DIR)/chess/piece/Piece.cpp $(SRC_DIR)/chess/algorithms/Algorithm.cpp
	$(CXX) -o $(TEST_EXECUTABLE) $(TEST_DIR)/chess/board/Board.test.cpp $(SRC_DIR)/chess/Chess.cpp $(SRC_DIR)/chess/board/Board.cpp $(SRC_DIR)/chess/piece/Piece.cpp $(SRC_DIR)/chess/algorithms/Algorithm.cpp
	valgrind --leak-check=yes ./$(TEST_EXECUTABLE)
	rm -f $(TEST_EXECUTABLE)

# Règle de nettoyage
clean:
	rm -f $(OBJ) $(EXECUTABLE) $(TEST_EXECUTABLE)
