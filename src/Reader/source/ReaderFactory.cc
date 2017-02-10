#include "ReaderFactory.h"
#include "XMLReader.h"
#include "XMLReaderConfig.h"
#include "XMLReaderElog.h"
#include "Global.h"
ReaderFactory::ReaderFactory()
{
  RegisterReader("XMLReader",new XMLReader());
  RegisterReader("XMLReaderConfig",new XMLReaderConfig());
  RegisterReader("XMLReaderElog",new XMLReaderElog());
}
