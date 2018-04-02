void testPrintAfterChange()
{
  givenAString();
  givenAChange();

  whenPrintingToStandardOutput();

  thenSuccess();
}

void testPrintToTerminal()
{
  givenAString();
  whenPrintingToStandardOutput();
  thenSuccess();
}

void testPrintArgumentToTerminal()
{
  whenPrintingToStandardOutput();
  thenSuccess();
}

void givenAString()
{
  setString("String");
}

void givenAChange()
{
  setString("Change");
}

void setString(char * stringValue)
{
}

void whenPrintingToStandardOutput()
{
}

void testConnection()
{
}


void testBufferConnection(key_t key)
{
  createConnectionWithSharedMemory(key);
}